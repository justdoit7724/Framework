#include "Object.h"
#include "TextureMgr.h"
#include "Light.h"
#include "Shader.h"
#include "Camera.h"

Object::Object()
{
}

Object::Object(Shape* shape, XMFLOAT3 mDiffuse, XMFLOAT3 mAmbient, XMFLOAT3 mSpec, float sP, XMFLOAT3 r, std::string texName, int zOrder)
	: zOrder(zOrder), shape(shape)
{
	transform = new Transform();

	vs = new VShader("StandardVS.cso", Std_ILayouts, ARRAYSIZE(Std_ILayouts));
	gs = new GShader();
	ps = new PShader("StandardPS.cso");

	vs->AddCB(0, 1, sizeof(VS_Property));
	ps->AddCB(0, 1, sizeof(SHADER_DIRECTIONAL_LIGHT));
	ps->AddCB(1, 1, sizeof(SHADER_POINT_LIGHT));
	ps->AddCB(2, 1, sizeof(SHADER_SPOT_LIGHT));
	ps->AddCB(3, 1, sizeof(XMFLOAT3));
	ps->AddCB(4, 1, sizeof(ShaderMaterial));
	ps->WriteCB(4,&ShaderMaterial(mDiffuse, 1, mAmbient, mSpec, sP, r));
	

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	ps->AddSamp(0, 1, &samplerDesc);
	ComPtr<ID3D11ShaderResourceView> srv = nullptr;
	TextureMgr::Instance()->Load(texName, 1, 1);
	TextureMgr::Instance()->Get(texName, &srv, nullptr);
	ps->AddSRV(0, 1, srv.Get());

	blendState = new BlendState();
	dsState = new DepthStencilState();

	shadow_vs = new VShader("ShadowVS.cso", Std_ILayouts, ARRAYSIZE(Std_ILayouts));
	shadow_ps = new PShader("ShadowPS.cso");
	shadow_vs->AddCB(0, 1, sizeof(VS_Simple_Property));
	shadow_ps->AddCB(0, 1, sizeof(float));
}

Object::~Object()
{
	delete transform;
	delete shape;
	delete vs;
	delete gs;
	delete ps;

	delete dsState;
	delete blendState;

	delete shadow_vs;
	delete shadow_ps;
}

void Object::EnableShadow(XMFLOAT3 shadowPlaneN, float _shadowPlaneDist, float _shadowTransparency)
{
	isShadow = true;
	shadowPlaneDist = _shadowPlaneDist;
	shadowPlaneNormal = shadowPlaneN;
	shadow_ps->WriteCB(0,&_shadowTransparency);
}


void Object::Update(Camera* camera, const SHADER_DIRECTIONAL_LIGHT* dLight, const SHADER_POINT_LIGHT* pLight, const SHADER_SPOT_LIGHT* sLight, const XMMATRIX& texMat)
{
	vpMat = camera->ViewMat()*camera->ProjMat(zOrder);
	XMMATRIX wMat = transform->WorldMatrix();

	vs->WriteCB(0,&VS_Property(wMat, vpMat, texMat));
	ps->WriteCB(0, (void*)dLight);
	ps->WriteCB(1, (void*)pLight);
	ps->WriteCB(2, (void*)sLight);
	ps->WriteCB(3, &(camera->Pos()));

	if (isShadow)
	{
		for (int i = 0; i < LIGHT_MAX_EACH; ++i)
		{
			if (DirectionalLight::Data()->enabled[i] != LIGHT_ENABLED)
				continue;

			XMFLOAT3 n = shadowPlaneNormal;
			float d = shadowPlaneDist;
			XMFLOAT3 l = XMFLOAT3(
				DirectionalLight::Data()->dir[i].x,
				DirectionalLight::Data()->dir[i].y,
				DirectionalLight::Data()->dir[i].z);
			float nl = Dot(n, l);
			dir_light_shadowMats[i] = XMMATRIX(
				nl - l.x*n.x, -l.y*n.x, -l.z*n.x, 0,
				-l.x*n.y, nl - l.y*n.y, -l.z*n.y, 0,
				-l.x*n.z, -l.y*n.z, nl - l.z*n.z, 0,
				l.x*d, l.y*d, l.z*d, nl
			);
		}
		for (int i = 0; i < LIGHT_MAX_EACH; ++i)
		{
			if (PointLight::Data()->enabled[i] != LIGHT_ENABLED)
				continue;

			XMFLOAT3 n = shadowPlaneNormal;
			float d = shadowPlaneDist;
			XMFLOAT3 l = XMFLOAT3(
				PointLight::Data()->pos[i].x,
				PointLight::Data()->pos[i].y,
				PointLight::Data()->pos[i].z);
			float nl = Dot(n, l);
			pt_light_shadowMats[i] = -XMMATRIX(
				nl - d - l.x*n.x,	-l.y*n.x,			-l.z*n.x,			-n.x,
				-l.x*n.y,			nl + d - l.y*n.y,	-l.z*n.y,			-n.y,
				-l.x*n.z,			-l.y*n.z,			nl + d - l.z*n.z,	-n.z,
				l.x*d,				l.y*d,				l.z*d,				nl
			);
		}
	}

	
}

void Object::Render()
{
	vs->Apply();
	gs->Apply();
	ps->Apply();

	// STATE
	dsState->Apply();
	blendState->Apply();
	shape->Apply();

	if (isShadow)
	{
		XMMATRIX wMat = transform->WorldMatrix();
		
		for (int i = 0; i < LIGHT_MAX_EACH; ++i)
		{
			if (DirectionalLight::Data()->enabled[i] != LIGHT_ENABLED)
				continue;

			shadow_vs->WriteCB(0,&VS_Simple_Property(wMat*dir_light_shadowMats[i], vpMat));
			shadow_vs->Apply();
			shadow_ps->Apply();

			shape->Apply();
		}
		for (int i = 0; i < LIGHT_MAX_EACH; ++i)
		{
			if (PointLight::Data()->enabled[i] != LIGHT_ENABLED)
				continue;

			shadow_vs->WriteCB(0,&VS_Simple_Property(wMat*pt_light_shadowMats[i], vpMat));
			shadow_vs->Apply();
			shadow_ps->Apply();

			shape->Apply();
		}
	}
}