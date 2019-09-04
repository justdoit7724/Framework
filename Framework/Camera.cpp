#include "Camera.h"
#include "Keyboard.h"
#include "Mouse.h"	
#include "Transform.h"

#define Z_ORDER_MAX 5

Camera::Camera(const FRAME_KIND	frameKind, const float orthoScnWidth, const float orthoScnHeight, const float n, const float f, const float verticalViewAngle, const float aspectRatio, const XMFLOAT3 firstPos, const XMFLOAT3 _forward, const XMFLOAT3 _up)
{
	SetFrame(frameKind, orthoScnWidth, orthoScnHeight, n, f, verticalViewAngle, aspectRatio);
	SetTransform(firstPos, _forward, _up);
}
void Camera::SetFrame(const FRAME_KIND fKind, const float screenWidth, const float screenHeight, const float n, const float f, const float verticalViewAngle, const float aspectRatio)
{
	assert(n > 0.0f);

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
				1.0f / (aspectRatio*tan(verticalViewAngle*0.5f)), 0, 0, 0,
				0, 1.0f / tan(verticalViewAngle*0.5f), 0, 0,
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
void Camera::SetTransform(const XMFLOAT3 _pos, const XMFLOAT3 _forward, const XMFLOAT3 _up)
{
	pos = _pos;
	forward = _forward;
	up = _up;
	right = Cross(up, forward);

	float x = -Dot(pos, right);
	float y = -Dot(pos, up);
	float z = -Dot(pos, forward);
	viewMat = XMMATRIX( // inverse of cam world matrix
		right.x, up.x, forward.x, 0,
		right.y, up.y, forward.y, 0,
		right.z, up.z, forward.z, 0,
		x, y, z, 1);
}
void Camera::Update(float spf)
{
	XMFLOAT3 newPos=pos;
	const float speed = 100;
	if (Keyboard::IsPressing('A')) {
		
		newPos += - right * speed * spf;
	}
	else if (Keyboard::IsPressing('D')) {

		newPos += right * speed * spf;
	}
	if (Keyboard::IsPressing('S')) {
		
		newPos += -forward * speed * spf;
	}
	else if (Keyboard::IsPressing('W')) {
		
		newPos += forward * speed * spf;
	}

	static float angleX = 0;
	static float angleY = 0;
	static XMFLOAT2 prevMousePt;
	const float angleSpeed = 3.141592f*0.2f;
	if (Mouse::IsRightDown())
	{
		angleY += angleSpeed * spf * (Mouse::X() - prevMousePt.x);
		angleX += angleSpeed * spf * (Mouse::Y() - prevMousePt.y);
	}

	prevMousePt.x = Mouse::X();
	prevMousePt.y = Mouse::Y();
	
	const XMMATRIX rotMat = XMMatrixRotationX(angleX) * XMMatrixRotationY(angleY);
	SetTransform(newPos, FORWARD * rotMat, UP*rotMat);
}
