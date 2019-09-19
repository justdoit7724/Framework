#include "Billboard.h"

static const D3D11_INPUT_ELEMENT_DESC bLayouts[] = {

	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(XMFLOAT3), D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

struct BVertex
{
	XMFLOAT3 pos;
	XMFLOAT2 size;
	BVertex() {}
	BVertex(XMFLOAT3 pos, XMFLOAT2 size)
		:pos(pos), size(size)
	{}
};
Billboard::Billboard(ID3D11ShaderResourceView* image)
	:Object()
{
	vs = new VShader("BillboardVS.cso", bLayouts, ARRAYSIZE(bLayouts));
	gs = new GShader("BillboardGS.cso");
	gs->AddCB(0, 1, sizeof(XMMATRIX));
	gs->AddCB(1, 1, sizeof(XMFLOAT3));
	ps = new PShader("BillboardPS.cso");
	ps->AddSRV(0, 1, image);
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
	shape = new Shape();

	dsState = new DepthStencilState();
	blendState = new BlendState();
}

void Billboard::Add(XMFLOAT3 pos)
{
	list.push_back(pos);

	std::vector<BVertex> vertice;
	for (auto p : list)
	{
		vertice.push_back(BVertex(p, XMFLOAT2(10, 20)));
	}
	std::vector<UINT> indice;
	for (int i = 0; i < vertice.size(); ++i)
	{
		indice.push_back(i);
	}

	shape->Init(vertice.data(), sizeof(BVertex), vertice.size(), indice.data(), indice.size(), D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
}

void Billboard::Update(XMMATRIX* vpMat, XMFLOAT3 eye)
{
	gs->WriteCB(0, vpMat);
	gs->WriteCB(1, &eye);
}