#include "PT_Obj.h"
#include "Shader.h"
#include "Transform.h"
#include "Camera.h"
#include "Light.h"

PT_Obj::PT_Obj(Shape* shape, const XMMATRIX& vp_mat, XMFLOAT3 dif, float transp, XMFLOAT3 amb, XMFLOAT3 spec, float sp, XMFLOAT3 refl, ID3D11ShaderResourceView* bodySRV, ID3D11ShaderResourceView* bodyNorm, ID3D11ShaderResourceView* ptSRV, ID3D11ShaderResourceView* cmSRV, int zOrder)
	:Object(
		shape,
		"ProjectiveTextureVS.cso", Std_ILayouts, ARRAYSIZE(Std_ILayouts),
		"","","",
		"ProjectiveTexturePS.cso", zOrder),
	vp_mat(vp_mat)
{
	vs->AddCB(0, 1, sizeof(SHADER_PT_TRANSF));
	ps->AddCB(0, 1, sizeof(SHADER_DIRECTIONAL_LIGHT));
	ps->AddCB(1, 1, sizeof(SHADER_POINT_LIGHT));
	ps->AddCB(2, 1, sizeof(SHADER_SPOT_LIGHT));
	ps->AddCB(3, 1, sizeof(XMFLOAT4));
	ps->AddCB(4, 1, sizeof(SHADER_MATERIAL));
	ps->AddCB(5, 1, sizeof(float));
	ps->WriteCB(4, &SHADER_MATERIAL(dif, transp, amb, spec, sp, refl));

	ps->AddSRV(0, 1);
	ps->AddSRV(1, 1);
	ps->AddSRV(2, 1);
	ps->AddSRV(3, 1);
	ps->WriteSRV(0, cmSRV);
	ps->WriteSRV(1, bodySRV);
	ps->WriteSRV(2, bodyNorm);
	ps->WriteSRV(3, ptSRV);

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(D3D11_SAMPLER_DESC));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	ps->AddSamp(0, 1, &sampDesc);
	ps->AddSamp(1, 1, &sampDesc);
}

void PT_Obj::Update(const Camera* camera, float elapsed, const XMMATRIX& texMat)
{
	vs->WriteCB(0, &SHADER_PT_TRANSF(transform->WorldMatrix(), camera->VPMat(zOrder), vp_mat, XMMatrixIdentity()));
	ps->WriteCB(0, DirectionalLight::Data());
	ps->WriteCB(1, PointLight::Data());
	ps->WriteCB(2, SpotLight::Data());
	ps->WriteCB(3, &(camera->transform->GetPos()));
	ps->WriteCB(5, &elapsed);
}