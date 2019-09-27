#include "CubeMap.h"
#include "Transform.h"
#include "Shader.h"
#include "Sphere.h"
#include "RasterizerState.h"
#include "Camera.h"
#include "DepthStencilState.h"
#include "InputLayoutBuilder.h"

CubeMap::CubeMap(ID3D11ShaderResourceView* srv)
{ 
	int* pI = TestBuilder().SetInput(1).SetInput(2).SetInput(3).Build();
	int a0 = pI[0];
	int a1 = pI[1];
	int a2 = pI[2];

	transform = new Transform();
	transform->SetScale(100, 75, 100);
	vs = new VShader("CMVS.cso", 
		InputLayoutBuilder().
		SetInput("POSITION", DXGI_FORMAT_R32G32B32_FLOAT, 0).
		SetInput("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT, sizeof(XMFLOAT3)).
		SetInput("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT, sizeof(XMFLOAT3)).Build(),
		3);
	vs->AddCB(0, 1, sizeof(XMMATRIX));
	hs = new HShader();
	ds = new DShader();
	gs = new GShader();
	ps = new PShader("CMPS.cso");
	ps->AddSRV(0, 1, srv);
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
	dsState = new DepthStencilState();
}

void CubeMap::Update(Camera* camera, const XMMATRIX& texMat)
{
	transform->SetTranslation(camera->Pos());
	XMMATRIX wvp = transform->WorldMatrix() * camera->VPMat(Z_ORDER_BACKGROUND);
	vs->WriteCB(0, &wvp);
}
