#include "Skybox.h"
#include "Transform.h"
#include "Shader.h"
#include "Sphere.h"
#include "RasterizerState.h"
#include "Camera.h"
#include "DepthStencilState.h"


Skybox::Skybox(ID3D11ShaderResourceView* srv)
	:Object(
		nullptr,
		"SkyboxVS.cso", Std_ILayouts,ARRAYSIZE(Std_ILayouts),
		"","","",
		"SkyboxPS.cso",Z_ORDER_STANDARD)
{ 
	transform->SetScale(1000, 700, 1000);
	
	vs->AddCB(0, 1, sizeof(XMMATRIX));
	ps->AddSRV(0, 1);
	ps->WriteSRV(0, srv);
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(D3D11_SAMPLER_DESC));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	ps->AddSamp(0, 1, &sampDesc);
	shape = new Sphere(5);
	D3D11_RASTERIZER_DESC rs_desc;
	ZeroMemory(&rs_desc, sizeof(D3D11_RASTERIZER_DESC));
	rs_desc.CullMode = D3D11_CULL_FRONT;
	rs_desc.FillMode = D3D11_FILL_SOLID;
	rs_desc.FrontCounterClockwise = false;
	rsState = new RasterizerState(&rs_desc);
}

void Skybox::Update(const Camera* camera, const XMMATRIX& texMat)
{
	transform->SetTranslation(camera->transform->GetPos());
	XMMATRIX wvp = transform->WorldMatrix() * camera->VPMat(zOrder);
	vs->WriteCB(0, &wvp);
}
