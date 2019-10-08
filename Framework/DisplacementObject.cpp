#include "DisplacementObject.h"
#include "Camera.h"
#include "Shader.h"
#include "Transform.h"
#include "Light.h"

DisplacementObject::DisplacementObject(
	Shape* shape, ID3D11ShaderResourceView* bodySRV, ID3D11ShaderResourceView* bodyNormal, ID3D11ShaderResourceView* dpSRV, float dp_scale,
	XMFLOAT3 mDiffuse, XMFLOAT3 mAmbient, XMFLOAT3 mSpec, float sp)
	:Object(
		shape, 
		"StdDisplacementVS.cso", Std_ILayouts, ARRAYSIZE(Std_ILayouts),
		"StdDisplacementHS.cso", "StdDisplacementDS.cso","",
		"StandardPS.cso", Z_ORDER_STANDARD),
	body_srv(bodySRV),
	normal_srv(bodyNormal),
	dpSRV(dpSRV),
	dp_scale(dp_scale)
{
	vs->AddCB(0, 1, sizeof(VS_Property));
	vs->AddCB(1, 1, sizeof(XMFLOAT3));
	ds->AddCB(0, 1, sizeof(XMMATRIX));
	ds->AddCB(1, 1, sizeof(float));
	ds->WriteCB(1, &dp_scale);
	ds->AddSRV(0, 1);
	ds->WriteSRV(0, dpSRV);
	D3D11_SAMPLER_DESC bumpSamp_desc;
	ZeroMemory(&bumpSamp_desc, sizeof(D3D11_SAMPLER_DESC));
	bumpSamp_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	bumpSamp_desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	bumpSamp_desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	bumpSamp_desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	bumpSamp_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	bumpSamp_desc.MinLOD = 0;
	bumpSamp_desc.MaxLOD = D3D11_FLOAT32_MAX;
	ds->AddSamp(0, 1, &bumpSamp_desc);
	ps->AddCB(0, 1, sizeof(SHADER_DIRECTIONAL_LIGHT));
	ps->AddCB(1, 1, sizeof(SHADER_POINT_LIGHT));
	ps->AddCB(2, 1, sizeof(SHADER_SPOT_LIGHT));
	ps->AddCB(3, 1, sizeof(XMFLOAT3));
	ps->AddCB(4, 1, sizeof(SHADER_MATERIAL));
	ps->WriteCB(4, &SHADER_MATERIAL(mDiffuse, 1, mAmbient, mSpec, sp, XMFLOAT3(0, 0, 0)));
	ps->AddSRV(0, 1);
	ps->AddSRV(1, 1);
	ps->AddSRV(2, 1);
	ps->WriteSRV(0, nullptr);
	ps->WriteSRV(1, bodySRV);
	ps->WriteSRV(2, bodyNormal);
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(D3D11_SAMPLER_DESC));
	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	ps->AddSamp(0, 1, &sampDesc);
	ps->AddSamp(1, 1, &sampDesc);
}

void DisplacementObject::Update(const Camera* camera, const XMMATRIX& texMat)
{
	XMFLOAT3 eyePos = camera->transform->GetPos();

	vs->WriteCB(0, &VS_Property(transform->WorldMatrix(), XMMatrixIdentity(), texMat));
	vs->WriteCB(1, &eyePos);
	ds->WriteCB(0, &(camera->VPMat(zOrder)));
	ps->WriteCB(0, DirectionalLight::Data());
	ps->WriteCB(1, PointLight::Data());
	ps->WriteCB(2, SpotLight::Data());
	ps->WriteCB(3, &eyePos);
}