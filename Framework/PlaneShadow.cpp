#include "PlaneShadow.h"
#include "Light.h"
#include "Camera.h"
#include "Shader.h"
#include "Transform.h"
#include "Shape.h"
#include "InputLayoutBuilder.h"

PlaneShadow::PlaneShadow(Shape* shape, XMFLOAT3 planeN, float planeD)
	: planeNormal(planeN), planeDist(planeD)
{
	vs = new VShader("ShadowVS.cso", 
		InputLayoutBuilder().
		SetInput("POSITION", DXGI_FORMAT_R32G32B32_FLOAT, 0).
		SetInput("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT, sizeof(XMFLOAT3)).
		SetInput("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT, sizeof(XMFLOAT3)).Build(),
		3);
	ps = new PShader("ShadowPS.cso");
	vs->AddCB(0, 1, sizeof(VS_Simple_Property));
	ps->AddCB(0, 1, sizeof(float));
	float transparency = 0.5f;
	ps->WriteCB(0, &transparency);
}

void PlaneShadow::Update(Camera* camera, const XMMATRIX& texMat)
{
	vpMat = camera->VPMat(2);

	XMFLOAT3 n = planeNormal;
	float d = planeDist;
	for (int i = 0; i < LIGHT_MAX_EACH; ++i)
	{
		if (DirectionalLight::Data()->enabled[i] != LIGHT_ENABLED)
			continue;

		XMFLOAT3 l = XMFLOAT3(
			DirectionalLight::Data()->dir[i].x,
			DirectionalLight::Data()->dir[i].y,
			DirectionalLight::Data()->dir[i].z);
		float nl = Dot(n, l);
		dir_light_shadowMats[i] = XMMATRIX(
			nl - l.x * n.x, -l.y * n.x, -l.z * n.x, 0,
			-l.x * n.y, nl - l.y * n.y, -l.z * n.y, 0,
			-l.x * n.z, -l.y * n.z, nl - l.z * n.z, 0,
			l.x * d, l.y * d, l.z * d, nl
		);
	}
	for (int i = 0; i < LIGHT_MAX_EACH; ++i)
	{
		if (PointLight::Data()->enabled[i] != LIGHT_ENABLED)
			continue;

		XMFLOAT3 l = XMFLOAT3(
			PointLight::Data()->pos[i].x,
			PointLight::Data()->pos[i].y,
			PointLight::Data()->pos[i].z);
		float nl = Dot(n, l);
		pt_light_shadowMats[i] = -XMMATRIX(
			nl - d - l.x * n.x, -l.y * n.x, -l.z * n.x, -n.x,
			-l.x * n.y, nl + d - l.y * n.y, -l.z * n.y, -n.y,
			-l.x * n.z, -l.y * n.z, nl + d - l.z * n.z, -n.z,
			l.x * d, l.y * d, l.z * d, nl
		);
	}
}

void PlaneShadow::Render()
{
	Object::Render();

	XMMATRIX wMat = transform->WorldMatrix();

	for (int i = 0; i < LIGHT_MAX_EACH; ++i)
	{
		if (DirectionalLight::Data()->enabled[i] != LIGHT_ENABLED)
			continue;

		vs->WriteCB(0, &VS_Simple_Property(wMat * dir_light_shadowMats[i], vpMat));
		vs->Apply();

		shape->Apply();
	}
	for (int i = 0; i < LIGHT_MAX_EACH; ++i)
	{
		if (PointLight::Data()->enabled[i] != LIGHT_ENABLED)
			continue;

		vs->WriteCB(0, &VS_Simple_Property(wMat * pt_light_shadowMats[i], vpMat));
		vs->Apply();

		shape->Apply();
	}
}
