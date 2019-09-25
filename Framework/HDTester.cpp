#include "HDTester.h"
#include "Quad.h"
#include "Camera.h"

HDTester::HDTester()
{
	shape = new Quad();
	std::vector<XMFLOAT3> vertice;
	vertice.push_back(XMFLOAT3(-0.5, -0.5, 0) * 200);
	vertice.push_back(XMFLOAT3(-0.5, 0.5, 0) * 200);
	vertice.push_back(XMFLOAT3(0.5, 0.5, 0) * 200);
	vertice.push_back(XMFLOAT3(0.5, -0.5, 0) * 200);
	std::vector<UINT> indice;
	indice.push_back(0);
	indice.push_back(1);
	indice.push_back(2);
	indice.push_back(3);
	shape->Init(vertice.data(), sizeof(XMFLOAT3), vertice.size(), indice.data(), indice.size(), D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);

	transform = new Transform();

	D3D11_INPUT_ELEMENT_DESC testLayouts[]=
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	vs = new VShader("VS.cso", testLayouts,1);
	hs = new HShader("HS.cso");
	hs->AddCB(0, 1, sizeof(XMFLOAT3));
	ds = new DShader("DS.cso");
	ds->AddCB(0, 1, sizeof(XMMATRIX));
	gs = new GShader();
	ps = new PShader("PS.cso");

	dsState = new DepthStencilState();
	blendState = new BlendState();
}

void HDTester::Update(Camera* camera, const XMMATRIX& texMat)
{
	hs->WriteCB(0, &camera->Pos());
	ds->WriteCB(0, &camera->VPMat(2));
}
