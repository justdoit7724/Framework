#include "Camera.h"
#include "Transform.h"

#define Z_ORDER_MAX 5

Camera::Camera(const FRAME_KIND	frameKind, UINT orthoScnWidth, UINT orthoScnHeight, const float n, const float f, const float verticalViewAngle, const float aspectRatio, const XMFLOAT3 firstPos, const XMFLOAT3 _forward, const XMFLOAT3 _up)
{
	transform = new Transform();

	SetFrame(frameKind, orthoScnWidth, orthoScnHeight, n, f, verticalViewAngle, aspectRatio);
	transform->SetTranslation(firstPos);
	transform->SetRot(_forward, _up);
}
void Camera::SetFrame(const FRAME_KIND fKind, UINT screenWidth, UINT screenHeight, const float n, const float f, const float verticalViewAngle, const float aspectRatio)
{
	assert(n > 0.0f);


	float verticalViewRad = verticalViewAngle * XM_PI / 180.0f;

	curFrame = fKind;

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
			float sX = 2.0f / (float)screenWidth;
			float sY = 2.0f / (float)screenHeight;
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
void Camera::Update(float spf)
{
	
}

XMMATRIX Camera::ViewMat()
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