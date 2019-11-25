#include "Skybox.h"
#include "Transform.h"
#include "Shader.h"
#include "Sphere.h"
#include "RasterizerState.h"
#include "Camera.h"
#include "DepthStencilState.h"
#include "ShaderFormat.h"
#include "CameraMgr.h"
#include "TextureMgr.h"
#include "ShaderReg.h"

Skybox::Skybox(ID3D11ShaderResourceView* srv)
	:Object(
		new Sphere(5),
		"CMVS.cso", Std_ILayouts,ARRAYSIZE(Std_ILayouts),
		"","","",
		"CMPS.cso",Z_ORDER_STANDARD)
{ 
	transform->SetScale(300, 300, 300);
	
	vs->AddCB(0, 1, sizeof(SHADER_STD_TRANSF));
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(D3D11_SAMPLER_DESC));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	r_assert(
		DX_Device->CreateSamplerState(&sampDesc, samplerState.GetAddressOf()));
	delete rsState;
	D3D11_RASTERIZER_DESC rs_desc;
	ZeroMemory(&rs_desc, sizeof(D3D11_RASTERIZER_DESC));
	rs_desc.CullMode = D3D11_CULL_FRONT;
	rs_desc.FillMode = D3D11_FILL_SOLID;
	rs_desc.FrontCounterClockwise = false;
	rsState = new RasterizerState(&rs_desc);
}

void Skybox::Mapping()
{
	ID3D11ShaderResourceView* cmSRV = TextureMgr::Instance()->Get("cm");
	DX_DContext->PSSetShaderResources(SHADER_REG_PS_SRV_CM, 1, &cmSRV);
	DX_DContext->PSSetSamplers(SHADER_REG_PS_SAMP_CM, 1, samplerState.GetAddressOf());
}

void Skybox::Update()
{
	transform->SetTranslation(CameraMgr::Instance()->Main()->transform->GetPos());
}


void Skybox::Render(const XMMATRIX& parentWorld, const XMMATRIX& vp, UINT sceneDepth) const
{
	Object::Render(parentWorld, vp, sceneDepth);
}


