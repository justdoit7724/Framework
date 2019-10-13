#include "Camera.h"
#include "Transform.h"
#include "CameraMgr.h"
#include "Scene.h"

#define Z_ORDER_MAX 5

Camera::Camera(std::string key, const Camera* camera)
{
	CameraMgr::Instance()->Add(key, this);

	transform = new Transform();

	SetFrame(camera->GetFrame(), camera->GetSize(), camera->GetN(), camera->GetF(), camera->GetVRad(), camera->GetAspectRatio());
	transform->SetTranslation(camera->transform->GetPos());
	transform->SetRot(camera->transform->GetRight(), camera->transform->GetUp(), camera->transform->GetForward());
}

Camera::Camera(std::string key, const FRAME_KIND frameKind, float orthoScnWidth, float orthoScnHeight, const float n, const float f, const float verticalViewAngle, const float aspectRatio, const XMFLOAT3 firstPos, const XMFLOAT3 _forward, const XMFLOAT3 _up)
	:key(key)
{
	CameraMgr::Instance()->Add(key, this);

	transform = new Transform();

	SetFrame(frameKind, XMFLOAT2(orthoScnWidth, orthoScnHeight), n, f, verticalViewAngle, aspectRatio);
	transform->SetTranslation(firstPos);
	transform->SetRot(_forward, _up, Cross(_up, _forward));
}

Camera::~Camera()
{
	CameraMgr::Instance()->Remove(key);
}
void Camera::SetMain()
{
	CameraMgr::Instance()->SetMain(key);
}
void Camera::SetFrame(const FRAME_KIND fKind, XMFLOAT2 orthoSize, const float n, const float f, const float verticalViewRad, const float aspectRatio)
{
	assert(n > 0.0f);

	curFrame = fKind;
	size = orthoSize;
	this->n = n;
	this->f = f;
	this->verticalRadian = verticalViewRad;
	this->aspectRatio = aspectRatio;

	projMats = new XMMATRIX[5];
	float interval = 1.0f / Z_ORDER_MAX;
	assert(Z_ORDER_MAX >= 1);
	switch (fKind)
	{
	case FRAME_KIND_PERSPECTIVE:
		for (int i = 0; i < Z_ORDER_MAX; ++i)
		{
			float minD = i * interval;
			float maxD = (i + 1) * interval;
			float A = maxD * f / (f - n) - minD * n / (f - n);
			float B = n * (minD - A);

			projMats[i] = XMMATRIX(
				1.0f / (aspectRatio*tan(verticalViewRad *0.5f)), 0, 0, 0,
				0, 1.0f / tan(verticalViewRad *0.5f), 0, 0,
				0, 0, A, 1,
				0, 0, B, 0);
		}
		break;
	case FRAME_KIND_ORTHOGONAL:
		for (int i = 0; i < Z_ORDER_MAX; ++i)
		{
			float sX = 2.0f / size.x;
			float sY = 2.0f / size.y;
			float sZ = interval / (f - n);
			float M = (-n / (f - n) + i) * interval;

			projMats[i] = XMMATRIX(
				sX, 0, 0, 0,
				0, sY, 0, 0,
				0, 0, sZ, 0,
				0, 0, M, 1
			);
		}
		break;
	}
	
}

void Camera::Capture(Scene* scene, ID3D11RenderTargetView** rtv, ID3D11DepthStencilView* dsv, D3D11_VIEWPORT vp)
{
	ID3D11RenderTargetView* oriRTV;
	ID3D11DepthStencilView* oriDSV;
	D3D11_VIEWPORT oriVP;
	DX_DContext->OMGetRenderTargets(1, &oriRTV, &oriDSV);
	UINT numVP = 1;
	DX_DContext->RSGetViewports(&numVP, &oriVP);

	DX_DContext->OMSetRenderTargets(1, rtv, dsv);
	DX_DContext->RSSetViewports(1, &vp);

	scene->Render_Update(this,0);
	scene->Render();

	DX_DContext->OMSetRenderTargets(1, &oriRTV, oriDSV);
	DX_DContext->RSSetViewports(1, &oriVP);
}

XMMATRIX Camera::ViewMat()const
{
	XMFLOAT3 pos = transform->GetPos();
	XMFLOAT3 forward = transform->GetForward();
	XMFLOAT3 up = transform->GetUp();
	XMFLOAT3 right = transform->GetRight();

	float x = -Dot(pos, right);
	float y = -Dot(pos, up);
	float z = -Dot(pos, forward);

	return XMMATRIX( // inverse of cam world matrix
		right.x, up.x, forward.x, 0,
		right.y, up.y, forward.y, 0,
		right.z, up.z, forward.z, 0,
		x, y, z, 1);
}