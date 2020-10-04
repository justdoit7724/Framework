
#include "pch.h"
#include "Camera.h"
#include "Transform.h"
#include "Mouse.h"
#include "Keyboard.h"

#define Z_ORDER_MAX 5
using namespace DX;

Camera::Camera(std::string key, FRAME_KIND frameKind, float orthoScnWidth, float orthoScnHeight, float n, float f, float verticalViewRad, float aspectRatio, bool skipFrustum)
	:key(key)
{
	transform = new Transform();

	SetView();
	SetFrame(frameKind, XMFLOAT2(orthoScnWidth, orthoScnHeight), n, f, verticalViewRad, aspectRatio);

	frustum.skip = skipFrustum;
}
Camera::~Camera()
{
	delete transform;
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

	float interval = 1.0f / Z_ORDER_MAX;
	assert(Z_ORDER_MAX >= 1);
	switch (fKind)
	{
	case FRAME_KIND_PERSPECTIVE:
	{
		float sX = 1.0f / (aspectRatio * tan(verticalViewRad * 0.5f));
		float sY = 1.0f / tan(verticalViewRad * 0.5f);
		projMat = XMMATRIX(
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
		projMat = XMMATRIX(
			sX, 0, 0, 0,
			0, sY, 0, 0,
			0, 0, 1.0f/(f-n), -n/(f-n),
			0, 0, 0, 1
		);
	}
		break;
	}
	
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

void Camera::SetProj(XMMATRIX mat)
{
	projMat = mat;
}

void Camera::Update()
{
	if (!frustum.skip)
	{
		XMFLOAT3 p = transform->GetPos();
		XMFLOAT3 forward = transform->GetForward();
		XMFLOAT3 up = transform->GetUp();
		XMFLOAT3 right = transform->GetRight();

		float tri = tan(verticalRadian * 0.5f);
		XMFLOAT3 trDir = Normalize(
			right * tri * f * aspectRatio +
			up * tri * f +
			forward * f);
		XMFLOAT3 blDir = Normalize(
			-right * tri * f * aspectRatio +
			-up * tri * f +
			forward * f);

		frustum.front = Geometrics::PlaneInf(p + forward * f, -forward);
		frustum.back = Geometrics::PlaneInf(p + forward * n, forward);
		frustum.left = Geometrics::PlaneInf(p, Cross(up, blDir));
		frustum.right = Geometrics::PlaneInf(p, Cross(-up, trDir));
		frustum.top = Geometrics::PlaneInf(p, Cross(right, trDir));
		frustum.bottom = Geometrics::PlaneInf(p, Cross(blDir, right));
	}

	SetView();
}
void Camera::Move(XMFLOAT2 scnPos, float spf)
{
	XMFLOAT3 newPos = transform->GetPos();
	XMFLOAT3 right = transform->GetRight();
	XMFLOAT3 forward = transform->GetForward();
	const float speed = 50;
	if (Keyboard::GetKey('A') == KeyState::KeyState_Pressing) {

		newPos += -right * speed * spf;
	}
	else if (Keyboard::GetKey('D') == KeyState::KeyState_Pressing) {

		newPos += right * speed * spf;
	}
	if (Keyboard::GetKey('S') == KeyState::KeyState_Pressing) {

		newPos += -forward * speed * spf;
	}
	else if (Keyboard::GetKey('W') == KeyState::KeyState_Pressing) {

		newPos += forward * speed * spf;
	}
	static float angleX = 0;
	static float angleY = 0;
	static XMFLOAT2 prevMousePt;
	const float angleSpeed = 3.141592f * 0.2f;
	/*if (Mouse::Instance()->RightState() == MOUSE_STATE_PRESSING)
	{
		angleY += angleSpeed * spf * (scnPos.x - prevMousePt.x);
		angleX += angleSpeed * spf * (scnPos.y - prevMousePt.y);
	}*/
	prevMousePt.x = scnPos.x;
	prevMousePt.y = scnPos.y;
	const XMMATRIX rotMat = XMMatrixRotationX(angleX) * XMMatrixRotationY(angleY);
	transform->SetTranslation(newPos);
	XMFLOAT3 f = MultiplyDir(FORWARD, rotMat);
	XMFLOAT3 u = MultiplyDir(UP, rotMat);
	transform->SetRot(f, u);
	Update();
}
void Camera::Pick(int iScnWidth, int iScnHeight, XMFLOAT2 scnPos, OUT Geometrics::Ray* ray)const
{
	XMFLOAT2 pPos = XMFLOAT2(
		((scnPos.x * 2) / (float)iScnWidth - 1),
		-(scnPos.y * 2) / (float)iScnHeight + 1);
	XMFLOAT3 vDir = XMFLOAT3(NULL,NULL,NULL);

	const XMFLOAT3 forward = transform->GetForward();
	const XMFLOAT3 up = transform->GetUp();
	const XMFLOAT3 right = transform->GetRight();
	const XMFLOAT3 eye = transform->GetPos();
	switch (curFrame)
	{
	case FRAME_KIND_PERSPECTIVE:
	{
		// vPos at z which is on d 
		vDir = Normalize(XMFLOAT3(
			pPos.x * aspectRatio* tan(verticalRadian * 0.5f),
			pPos.y* tan(verticalRadian * 0.5f),
			1));

		XMMATRIX invVDirMat = XMMATRIX(
			right.x, right.y, right.z, 0,
			up.x, up.y, up.z, 0,
			forward.x, forward.y, forward.z, 0,
			eye.x, eye.y, eye.z, 1);
		XMMATRIX id = VMat() * invVDirMat;
		//invVDirMat = XMMatrixInverse(&XMMatrixDeterminant(VMat()), VMat());

		ray->o = eye;
		ray->d = MultiplyDir(vDir, invVDirMat);
	}
		break;
	case FRAME_KIND_ORTHOGONAL:
	{
		float invSX = size.x/2;
		float invSY = size.y/2;
		XMFLOAT3 vPos = XMFLOAT3(
			pPos.x * invSX,
			pPos.y * invSY,
			1);
		vDir = Normalize(vPos);

		ray->o = eye + right * vPos.x + up * vPos.y;

		ray->d = forward;
	}
		break;
	default:
		break;
	}


	
}
