#include "ShadowObj.h"
#include "Shader.h"
#include "TextureMgr.h"
#include "Transform.h"
#include "Camera.h"
#include "ShaderFormat.h"

ShadowObj::ShadowObj(Shape* shape, ID3D11ShaderResourceView* bodySRV, ID3D11ShaderResourceView* bodyNormal, int zOrder)
	:Object(shape, "StdShadowVS.cso", Std_ILayouts, ARRAYSIZE(Std_ILayouts), "", "", "", "StdShadowPS.cso", zOrder)
{
	vs->AddCB(0, 1, sizeof(SHADER_PT_TRANSF));
	ps->AddCB(3, 1, sizeof(XMFLOAT4));
	ps->AddCB(4, 1, sizeof(SHADER_MATERIAL));
	ps->AddCB(5, 1, sizeof(float));
	ps->WriteCB(4, &SHADER_MATERIAL(XMFLOAT3(1,1,1), 1, XMFLOAT3(1, 1, 1), XMFLOAT3(1, 1, 1), 4, XMFLOAT3(0, 0, 0)));


	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	ps->AddSamp(0, 1, &samplerDesc);
	ps->AddSamp(1, 1, &samplerDesc);
	D3D11_SAMPLER_DESC shadowSampDesc;
	ZeroMemory(&shadowSampDesc, sizeof(D3D11_SAMPLER_DESC));
	shadowSampDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	shadowSampDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	shadowSampDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	shadowSampDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	shadowSampDesc.BorderColor[0] = 0;
	shadowSampDesc.BorderColor[1] = 0;
	shadowSampDesc.BorderColor[2] = 0;
	shadowSampDesc.BorderColor[3] = 0;
	shadowSampDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	shadowSampDesc.MinLOD = 0;
	shadowSampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	ps->AddSamp(2, 1, &shadowSampDesc);

	ps->AddSRV(0, 1);
	ps->AddSRV(1, 1);
	ps->AddSRV(2, 1);
	ps->AddSRV(3, 1);
	ps->WriteSRV(0, nullptr);
	ps->WriteSRV(1, bodySRV);
	ps->WriteSRV(2, bodyNormal);

}

void ShadowObj::Update(const Camera* camera, float elapsed, const XMMATRIX& shadowVP)
{
	const SHADER_PT_TRANSF STransformation(transform->WorldMatrix(), camera->VPMat(zOrder), shadowVP, XMMatrixIdentity());

	XMFLOAT3 eye = camera->GetPos();

	vs->WriteCB(0, (void*)(&STransformation));
	ps->WriteCB(3, &XMFLOAT4(eye.x, eye.y, eye.z, 0));
	ps->WriteCB(5, &elapsed);
}
