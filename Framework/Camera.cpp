#include "Camera.h"
#include "Transform.h"
#include "CameraMgr.h"
#include "Scene.h"
#include "Debugging.h"

#define Z_ORDER_MAX 5

Camera::Camera(std::string key, const Camera* camera)
{
	CameraMgr::Instance()->Add(key, this);

	transform = new Transform();

	SetFrame(camera->GetFrame(), camera->GetSize(), camera->GetN(), camera->GetF(), camera->GetVRad(), camera->GetAspectRatio());
	SetPos(camera->transform->GetPos());
	SetRot(camera->transform->GetForward(), camera->transform->GetUp());
}

Camera::Camera(std::string key, FRAME_KIND frameKind, float orthoScnWidth, float orthoScnHeight, float n, float f, float verticalViewRad, float aspectRatio)
	:key(key)
{
	CameraMgr::Instance()->Add(key, this);

	transform = new Transform();

	SetView();
	SetFrame(frameKind, XMFLOAT2(orthoScnWidth, orthoScnHeight), n, f, verticalViewRad, aspectRatio);
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
	{
		float sX = 1.0f / (aspectRatio * tan(verticalViewRad * 0.5f));
		float sY = 1.0f / tan(verticalViewRad * 0.5f);
		for (int i = 0; i < Z_ORDER_MAX; ++i)
		{
			float minD = i * interval;
			float maxD = (i + 1) * interval;
			float A = maxD * f / (f - n) - minD * n / (f - n);
			float B = n * (minD - A);

			projMats[i] = XMMATRIX(
				sX, 0, 0, 0,
				0, sY, 0, 0,
				0, 0, A, 1,
				0, 0, B, 0);
		}
		stdProjMat = XMMATRIX(
			sX, 0, 0, 0,
			0, sY, 0, 0,
			0, 0, f / (f - n), 1,
			0, 0, -n * f / (f - n), 0);
	}
		break;
	case FRAME_KIND_ORTHOGONAL:
	{
		float sX = 2.0f / size.x;
		float sY = 2.0f / size.y;
		for (int i = 0; i < Z_ORDER_MAX; ++i)
		{
			float sZ = interval / (f - n);
			float M = (-n / (f - n) + i) * interval;

			projMats[i] = XMMATRIX(
				sX, 0, 0, 0,
				0, sY, 0, 0,
				0, 0, sZ, 0,
				0, 0, M, 1
			);
		}
		stdProjMat = XMMATRIX(
			sX, 0, 0, 0,
			0, sY, 0, 0,
			0, 0, 1.0f / (f - n), 0,
			0, 0, -n / (f - n), 1
		);
	}
		break;
	}
	
}

XMFLOAT3 Camera::GetForward() const
{
	return transform->GetForward();
}

XMFLOAT3 Camera::GetRight() const
{
	return transform->GetRight();
}

XMFLOAT3 Camera::GetPos() const
{
	return transform->GetPos();
}

void Camera::SetView()
{
	XMFLOAT3 pos = transform->GetPos();
	XMFLOAT3 forward = transform->GetForward();
	XMFLOAT3 up = transform->GetUp();
	XMFLOAT3 right = transform->GetRight();

	float x = -Dot(pos, right);
	float y = -Dot(pos, up);
	float z = -Dot(pos, forward);

	viewMat = XMMATRIX( // inverse of cam world matrix
		right.x, up.x, forward.x, 0,
		right.y, up.y, forward.y, 0,
		right.z, up.z, forward.z, 0,
		x, y, z, 1);
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

	scene->Render_Update(this,0,0);
	scene->Render();

	DX_DContext->OMSetRenderTargets(1, &oriRTV, oriDSV);
	DX_DContext->RSSetViewports(1, &oriVP);
}
void Camera::Volume()
{
	XMFLOAT3 p = transform->GetPos();
	XMFLOAT3 forward = transform->GetForward();
	XMFLOAT3 up = transform->GetUp();
	XMFLOAT3 right = transform->GetRight();
	XMFLOAT3 sTL=XMFLOAT3(0,0,0);
	XMFLOAT3 sTR=XMFLOAT3(0,0,0);
	XMFLOAT3 sBL=XMFLOAT3(0,0,0);
	XMFLOAT3 sBR=XMFLOAT3(0,0,0);
	XMFLOAT3 eTL=XMFLOAT3(0,0,0);
	XMFLOAT3 eTR=XMFLOAT3(0,0,0);
	XMFLOAT3 eBL=XMFLOAT3(0,0,0);
	XMFLOAT3 eBR=XMFLOAT3(0,0,0);
	switch (curFrame)
	{
	case FRAME_KIND_PERSPECTIVE:
	{
		float tri = tan(verticalRadian * 0.5f);
		float nY = tri * n;
		float nX = nY * aspectRatio;
		float fY = tri * f;
		float fX = fY * aspectRatio;
		sTL = p + right * -nX + up * nY + forward * n;
		sTR = p + right * nX + up * nY + forward * n;
		sBL = p + right * -nX + up * -nY + forward * n;
		sBR = p + right * nX + up * -nY + forward * n;
		eTL = p + right * -fX + up * fY + forward * f;
		eTR = p + right * fX + up * fY + forward * f;
		eBL = p + right * -fX + up * -fY + forward * f;
		eBR = p + right * fX + up * -fY + forward * f;
	}
		break;
	case FRAME_KIND_ORTHOGONAL:
	{
		float x = size.x * 0.5f;
		float y = size.y * 0.5f;
		sTL = p + right * -x + up * y + forward * n;
		sTR = p + right * x + up * y + forward * n;
		sBL = p + right * -x + up * -y + forward * n;
		sBR = p + right * x + up * -y + forward * n;
		eTL = p + right * -x + up * y + forward * f;
		eTR = p + right * x + up * y + forward * f;
		eBL = p + right * -x + up * -y + forward * f;
		eBR = p + right * x + up * -y + forward * f;
	}
	break;
	}
	
	Debugging::Instance()->PtLine(sTL, sTR);
	Debugging::Instance()->PtLine(sTR, sBR);
	Debugging::Instance()->PtLine(sBR, sBL);
	Debugging::Instance()->PtLine(sBL, sTL);

	Debugging::Instance()->PtLine(sTL, eTL);
	Debugging::Instance()->PtLine(sTR, eTR);
	Debugging::Instance()->PtLine(sBL, eBL);
	Debugging::Instance()->PtLine(sBR, eBR);

	Debugging::Instance()->PtLine(eTL, eTR);
	Debugging::Instance()->PtLine(eTR, eBR);
	Debugging::Instance()->PtLine(eBR, eBL);
	Debugging::Instance()->PtLine(eBL, eTL);
}
void Camera::SetPos(XMFLOAT3 pos)
{
	transform->SetTranslation(pos);
	SetView();
}

void Camera::SetRot(XMFLOAT3 forward)
{
	transform->SetRot(forward);
	SetView();
}
void Camera::SetRot(XMFLOAT3 forward, XMFLOAT3 up)
{
	transform->SetRot(forward, up);
	SetView();
}
