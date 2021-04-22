
#include "stdafx.h"
#include "Camera.h"
#include "Transform.h"
#include "Object.h"

#define Z_ORDER_MAX 5

Camera* mainCamera=nullptr;

Camera::Camera(Object* owner)
	:
	Component(CMPNT_KIND::Camera, owner)
{
	SetView();
	SetFrame(FRAME_KIND_PERSPECTIVE, XMFLOAT2(800, 800), m_near, m_far, XM_PIDIV2, 1);
}
Camera::~Camera()
{

}

void Camera::SetMain(Camera* cam)
{
	mainCamera = cam;
}

const Camera* Camera::Main()
{
	return mainCamera;
}

void Camera::Update(DX::Graphic* graphic)
{
	SetView();

	SetFrame(curFrame, m_size, m_near, m_far, verticalRadian, aspectRatio);

	SetFrustum(m_near, m_far);

}

void Camera::Render(DX::Graphic* graphic)const
{
	std::vector<const void*> datas;
	datas.push_back(&m_view);
	graphic->SetPipelineData(PIP_REG::TRANSFORM_VIEW, datas);

	datas.clear();
	datas.push_back(&m_proj);
	graphic->SetPipelineData(PIP_REG::TRANSFORM_PROJECT, datas);
}

void Camera::SetView()
{
	Transform* tf = (Transform*)m_object->GetComponent(CMPNT_KIND::Transform);

	XMFLOAT3 pos = tf->GetPos();
	XMFLOAT3 forward = tf->GetForward();
	XMFLOAT3 up = tf->GetUp();
	XMFLOAT3 right = tf->GetRight();

	float x = -Dot(pos, right);
	float y = -Dot(pos, up);
	float z = -Dot(pos, forward);

	m_view = XMMATRIX( // inverse of cam world matrix
		right.x, up.x, forward.x, 0,
		right.y, up.y, forward.y, 0,
		right.z, up.z, forward.z, 0,
		x, y, z, 1);
}
void Camera::SetFrame(const FRAME_KIND fKind, XMFLOAT2 orthoSize, const float n, const float f, const float verticalViewRad, const float aspectRatio)
{
	assert(n > 0.0f);

	curFrame = fKind;
	m_size = orthoSize;
	this->m_near = n;
	this->m_far = f;
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
		m_proj = XMMATRIX(
			sX, 0, 0, 0,
			0, sY, 0, 0,
			0, 0, f / (f - n), 1,
			0, 0, -n * f / (f - n), 0);
	}
	break;
	case FRAME_KIND_ORTHOGONAL:
	{
		float sX = 2.0f / m_size.x;
		float sY = 2.0f / m_size.y;
		m_proj = XMMATRIX(
			sX, 0, 0, 0,
			0, sY, 0, 0,
			0, 0, 1.0f / (f - n), 0,
			0, 0, -n / (f - n), 1
		);
	}
	break;
	}

}

void Camera::SetFrustum(float n, float f)
{
	m_near = n;
	m_far = f;

	Transform* tf = (Transform*)m_object->GetComponent(CMPNT_KIND::Transform);
	XMFLOAT3 p = tf->GetPos();
	XMFLOAT3 forward = tf->GetForward();
	XMFLOAT3 up = tf->GetUp();
	XMFLOAT3 right = tf->GetRight();
	float tri = tan(verticalRadian * 0.5f);
	XMFLOAT3 trDir = Normalize(
		right * tri * f * aspectRatio +
		up * tri * f +
		forward * f);
	XMFLOAT3 blDir = Normalize(
		-right * tri * f * aspectRatio +
		-up * tri * f +
		forward * f);
	frustum.front = Geom::PlaneInf(p + forward * f, -forward);
	frustum.back = Geom::PlaneInf(p + forward * n, forward);
	frustum.left = Geom::PlaneInf(p, Cross(up, blDir));
	frustum.right = Geom::PlaneInf(p, Cross(-up, trDir));
	frustum.top = Geom::PlaneInf(p, Cross(right, trDir));
	frustum.bottom = Geom::PlaneInf(p, Cross(blDir, right));
}

void Camera::Pick(XMFLOAT2 scnPos, OUT Geom::Ray* ray)const
{
	XMFLOAT2 pPos = XMFLOAT2(
		((scnPos.x * 2) / (float)m_size.x - 1),
		-(scnPos.y * 2) / (float)m_size.y + 1);
	XMFLOAT3 vDir = XMFLOAT3(NULL, NULL, NULL);

	Transform* tf = (Transform*)m_object->GetComponent(CMPNT_KIND::Transform);

	const XMFLOAT3 forward = tf->GetForward();
	const XMFLOAT3 up = tf->GetUp();
	const XMFLOAT3 right = tf->GetRight();
	const XMFLOAT3 eye = tf->GetPos();
	switch (curFrame)
	{
	case FRAME_KIND_PERSPECTIVE:
	{
		// vPos at z which is on d 
		vDir = Normalize(XMFLOAT3(
			pPos.x * aspectRatio * tan(verticalRadian * 0.5f),
			pPos.y * tan(verticalRadian * 0.5f),
			1));

		XMMATRIX invVDirMat = XMMATRIX(
			right.x, right.y, right.z, 0,
			up.x, up.y, up.z, 0,
			forward.x, forward.y, forward.z, 0,
			eye.x, eye.y, eye.z, 1);

		ray->o = eye;
		ray->d = MultiplyDir(vDir, invVDirMat);
	}
	break;
	case FRAME_KIND_ORTHOGONAL:
	{
		float invSX = m_size.x / 2;
		float invSY = m_size.y / 2;
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
