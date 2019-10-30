#include "TestScene.h"
#include "Game_info.h"
#include "Camera.h"
#include "Keyboard.h"
#include "Mouse.h"	
#include "TextureMgr.h"
#include "Camera.h"
#include "ShaderFormat.h"
#include "UI.h"
#include "Object.h"
#include "Geometrics.h"
#include "Network.h"
#include "DepthStencilState.h"
#include "BlendState.h"
#include "RasterizerState.h"

#include "Hill.h"
#include "Sphere.h"
#include "Quad.h"
#include "Cube.h"
#include "Cylinder.h"
#include "Shader.h"
#include "Light.h"
#include "Texture2D.h"
#include "Buffer.h"
#include "Skybox.h"
#include "Transform.h"
#include "Timer.h"
#include "CameraMgr.h"
#include "DisplacementObject.h"
#include "PT_Obj.h"
#include "ShadowMap.h"
#include "ShadowObj.h"
#include "DynamicCubeMap.h"
#include "Debugging.h"
#include "nanosuit.h"

ID3D11Texture2D* aoMap;
ID3D11ShaderResourceView* aoSRV;
ID3D11RenderTargetView* aoRTV;
ID3D11DepthStencilView* aoDSV;
VShader* aoMapVS;
PShader* aoMapPS;
VShader* aoVS;
PShader* aoPS;
DepthStencilState* dsState;
DepthStencilState* noDsState;
BlendState* blendState;
D3D11_VIEWPORT aoVp; 
Object* flor;

struct Vertex_AO
{
	XMFLOAT3 pPos;
	float farPlaneIdx;
	XMFLOAT2 tex;
	Vertex_AO(XMFLOAT3 p,float id, XMFLOAT2 t) :pPos(p), farPlaneIdx(id), tex(t) {}
};
static Vertex_AO aoVertice[4] = {
	Vertex_AO(XMFLOAT3(-1, 1,1),0,XMFLOAT2(0,0)),
	Vertex_AO(XMFLOAT3( 1, 1,1),1,XMFLOAT2(1,0)),
	Vertex_AO(XMFLOAT3(-1,-1,1),2,XMFLOAT2(0,1)),
	Vertex_AO(XMFLOAT3( 1,-1,1),3,XMFLOAT2(1,1)) };
static XMFLOAT4 vFarPlane[4] = {
	XMFLOAT4(-1000 * tan(XM_PIDIV4),  1000 * tan(XM_PIDIV4),1000,0),
	XMFLOAT4( 1000 * tan(XM_PIDIV4),  1000 * tan(XM_PIDIV4),1000,0),
	XMFLOAT4(-1000 * tan(XM_PIDIV4), -1000 * tan(XM_PIDIV4),1000,0),
	XMFLOAT4( 1000 * tan(XM_PIDIV4), -1000 * tan(XM_PIDIV4),1000,0) };
ID3D11Buffer* aoVB;
nanosuit* mesh;
static XMFLOAT4 sample[14] = {
	XMFLOAT4(+1.0f, +1.0f, +1.0f, 0.0f),
	XMFLOAT4(-1.0f, -1.0f, -1.0f, 0.0f),
	XMFLOAT4(-1.0f, +1.0f, +1.0f, 0.0f),
	XMFLOAT4(+1.0f, -1.0f, -1.0f, 0.0f),
	XMFLOAT4(+1.0f, +1.0f, -1.0f, 0.0f),
	XMFLOAT4(-1.0f, -1.0f, +1.0f, 0.0f),
	XMFLOAT4(-1.0f, +1.0f, -1.0f, 0.0f),
	XMFLOAT4(+1.0f, -1.0f, +1.0f, 0.0f),
	XMFLOAT4(-1.0f, 0.0f, 0.0f, 0.0f),
	XMFLOAT4(+1.0f, 0.0f, 0.0f, 0.0f),
	XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f),
	XMFLOAT4(0.0f, +1.0f, 0.0f, 0.0f),
	XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f),
	XMFLOAT4(0.0f, 0.0f, +1.0f, 0.0f) };

TestScene::TestScene(IGraphic* graphic)
	:Scene("Test"),
	graphic(graphic)
{
	timer = new Timer();
	canvas = new UICanvas(SCREEN_WIDTH, SCREEN_HEIGHT);
	//Debugging::Instance()->EnableGrid(10, 50);

	/*dLight = new DirectionalLight(
		XMFLOAT3(0.1f, 0.1f, 0.1f),
		XMFLOAT3(0.9f, 0.9f, 0.9f),
		XMFLOAT3(0.9f, 0.9f, 0.9f),
		XMFLOAT3(0.707f, -0.707f, 0));*/
	pLight = new PointLight(
		XMFLOAT3(0.1f, 0.1f, 0.1f),
		XMFLOAT3(0.9f, 0.9f, 0.9f),
		XMFLOAT3(0.8f, 0.8f, 0.8f),
		200, XMFLOAT3(0.01f, 0.001f, 0.0005f), XMFLOAT3(0,0,0)
	);
	/*pLight2 = new PointLight(
		XMFLOAT3(0.15f, 0.15f, 0.15f),
		XMFLOAT3(0.6f, 0.6f, 0.6f),
		XMFLOAT3(0.7f, 0.7f, 0.7f),
		XMFLOAT3(0, 0, 0), 200, XMFLOAT3(0.05f, 0.01f, 0.001f)
	);*/

	TextureMgr::Instance()->Load("rock" ,"Data\\Texture\\rock.jpg");
	TextureMgr::Instance()->Load("rock_normal", "Data\\Texture\\rock_normal.jpg");
	TextureMgr::Instance()->Load("simple", "Data\\Texture\\sample.jpg");
	ID3D11ShaderResourceView* pbrSRV= TextureMgr::Instance()->Get("rock");
	ID3D11ShaderResourceView* pbrNormal= TextureMgr::Instance()->Get("rock_normal");;
	ID3D11ShaderResourceView* simpleSRV= TextureMgr::Instance()->Get("simple");;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////

	D3D11_TEXTURE2D_DESC aoTex_desc;
	aoTex_desc.ArraySize = 1;
	aoTex_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	aoTex_desc.CPUAccessFlags = 0;
	aoTex_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	aoTex_desc.Width = SCREEN_WIDTH/2;
	aoTex_desc.Height = SCREEN_HEIGHT/2;
	aoTex_desc.MipLevels = 1;
	aoTex_desc.MiscFlags = 0;
	aoTex_desc.SampleDesc = { 1,0 };
	aoTex_desc.Usage = D3D11_USAGE_DEFAULT;
	r_assert(
		DX_Device->CreateTexture2D(&aoTex_desc, nullptr, &aoMap)
	);
	D3D11_RENDER_TARGET_VIEW_DESC aoRTV_desc;
	aoRTV_desc.Format = aoTex_desc.Format;
	aoRTV_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	aoRTV_desc.Texture2D.MipSlice = 0;
	r_assert(
		DX_Device->CreateRenderTargetView(aoMap, &aoRTV_desc, &aoRTV)
	);
	D3D11_SHADER_RESOURCE_VIEW_DESC aoSRV_desc;
	aoSRV_desc.Format = aoTex_desc.Format;
	aoSRV_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	aoSRV_desc.Texture2D.MipLevels = 1;
	aoSRV_desc.Texture2D.MostDetailedMip = 0;
	r_assert(
		DX_Device->CreateShaderResourceView(aoMap, &aoSRV_desc, &aoSRV)
	);
	D3D11_TEXTURE2D_DESC aoDepth_desc;
	aoDepth_desc.ArraySize = 1;
	aoDepth_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	aoDepth_desc.CPUAccessFlags = 0;
	aoDepth_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	aoDepth_desc.Width = aoTex_desc.Width;
	aoDepth_desc.Height = aoTex_desc.Height;
	aoDepth_desc.MipLevels = 1;
	aoDepth_desc.MiscFlags = 0;
	aoDepth_desc.SampleDesc = { 1,0 };
	aoDepth_desc.Usage = D3D11_USAGE_DEFAULT;
	ID3D11Texture2D* aoDepth;
	r_assert(
		DX_Device->CreateTexture2D(&aoDepth_desc, nullptr, &aoDepth)
	);
	D3D11_DEPTH_STENCIL_VIEW_DESC aoDSV_desc;
	aoDSV_desc.Flags = 0;
	aoDSV_desc.Format = aoDepth_desc.Format;
	aoDSV_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	aoDSV_desc.Texture2D.MipSlice = 0;
	r_assert(
		DX_Device->CreateDepthStencilView(aoDepth, &aoDSV_desc, &aoDSV)
	);
	aoMapVS = new VShader("AOVS.cso", Std_ILayouts, ARRAYSIZE(Std_ILayouts));
	aoMapVS->AddCB(0, 1, sizeof(XMMATRIX) * 4);
	aoMapPS = new PShader("AOPS.cso");
	static D3D11_INPUT_ELEMENT_DESC aoILayout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "FAR_PLANE_IDX", 0,DXGI_FORMAT_R32_FLOAT, 0, sizeof(XMFLOAT3),D3D11_INPUT_PER_VERTEX_DATA,0},
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(XMFLOAT4), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	aoVS = new VShader("AO2VS.cso", aoILayout, ARRAYSIZE(aoILayout));
	aoVS->AddCB(0, 1, sizeof(XMFLOAT4) * 4);
	aoVS->WriteCB(0, vFarPlane);
	aoPS = new PShader("AO2PS.cso");
	aoPS->AddCB(0, 1, sizeof(XMMATRIX));
	aoPS->AddCB(1, 1, sizeof(XMFLOAT4) * 14);
	aoPS->AddSRV(0, 1);
	D3D11_SAMPLER_DESC aoSamp_desc;
	ZeroMemory(&aoSamp_desc, sizeof(D3D11_SAMPLER_DESC));
	aoSamp_desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	aoSamp_desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	aoSamp_desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	aoSamp_desc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	aoSamp_desc.BorderColor[0] = 1;
	aoSamp_desc.BorderColor[1] = 1;
	aoSamp_desc.BorderColor[2] = 1;
	aoSamp_desc.BorderColor[3] = 100000;
	aoPS->AddSamp(0, 1, &aoSamp_desc);
	dsState = new DepthStencilState(nullptr);
	D3D11_DEPTH_STENCIL_DESC noDS_desc;
	ZeroMemory(&noDS_desc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	noDS_desc.DepthEnable = false;
	noDsState = new DepthStencilState(&noDS_desc);
	blendState = new BlendState(nullptr);

	D3D11_BUFFER_DESC aoVB_desc;
	aoVB_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	aoVB_desc.ByteWidth = sizeof(Vertex_AO) * 4;
	aoVB_desc.CPUAccessFlags = 0;
	aoVB_desc.MiscFlags = 0;
	aoVB_desc.StructureByteStride = 0;
	aoVB_desc.Usage = D3D11_USAGE_DEFAULT;
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = aoVertice;
	r_assert(
		DX_Device->CreateBuffer(&aoVB_desc, &data, &aoVB));
	aoVp.Width = aoTex_desc.Width;
	aoVp.Height = aoTex_desc.Height;
	aoVp.TopLeftX = 0;
	aoVp.TopLeftY = 0;
	aoVp.MinDepth = 0;
	aoVp.MaxDepth = 1;
	
	const int N = 1;
	for (int z = 0; z < N; ++z) {
		for (int y = 0; y < 2; ++y) {
			for (int x = 0; x < N; ++x) {
				float interval = 20;
				XMFLOAT3 scale = XMFLOAT3(10, 10, 10);
				XMFLOAT3 offset = XMFLOAT3(scale.x,0,scale.z)* (N / 2);
				Object* cube = new Object(new Cube(), pbrSRV, pbrNormal);
				cube->transform->SetScale(scale);
				cube->transform->SetTranslation(XMFLOAT3(x * interval,7.5f + y * interval, z * interval)-offset);
				//AddObj(cube);
			}
		}
	}
	flor = new Object(new Quad(), pbrSRV, pbrNormal);
	flor->transform->SetScale(100, 100, 1);
	flor->transform->SetRot(-FORWARD, UP);
	AddObj(flor);

	mesh = new nanosuit();
	mesh->SetScale(XMFLOAT3(3, 3, 3));

	Debugging::Instance()->EnableGrid(10);
}

TestScene::~TestScene()
{
	delete canvas;
	delete dLight;
}


void TestScene::Update(float elapsed, float spf)
{
	pLight->Update();
	timer->Update();

	float elaped = timer->Elapsed();
	if (dLight)
	{
		XMFLOAT3 pt = XMFLOAT3(
			150,
			150,
			0) * XMMatrixRotationY(elaped * 0.1f);
		XMFLOAT3 dir = Normalize(XMFLOAT3(-pt.x, -pt.y, -pt.z));
		dLight->SetDir(dir);
	}
	if (pLight)
	{
		XMFLOAT3 pt = XMFLOAT3(
			cos(elaped * 0.6f) * 20,
			25,
			sin(elaped * 0.6f) * 20);
		pLight->SetPos(pt);
		//Debugging::Instance()->Mark(pt, 1.5f, Colors::WhiteSmoke);
	}
	if (pLight2)
	{
		XMFLOAT3 pt = XMFLOAT3(
			cos(elaped * 0.15f) * 15,
			cos(elaped) * 3,
			sin(elaped * 0.15f) * 25);
		pLight2->SetPos(pt);
		Debugging::Instance()->Mark(pt, 1.5f, Colors::Red);
	}

	FrustumCulling(CameraMgr::Instance()->Main());
	Scene::Update(elaped, spf);

	// first -------------------------------------------------------------
	XMMATRIX projMat= CameraMgr::Instance()->Main()->ProjMat(Z_ORDER_STANDARD);
	const float defaultColor[4] = { 1,1,1,100000 };
	ID3D11ShaderResourceView* nullSRV = nullptr;
	DX_DContext->RSSetViewports(1, &aoVp);
	DX_DContext->PSSetShaderResources(0, 1, &nullSRV);
	DX_DContext->ClearRenderTargetView(aoRTV, defaultColor);
	DX_DContext->ClearDepthStencilView(aoDSV, D3D11_CLEAR_DEPTH, 1.0, NULL);
	DX_DContext->OMSetRenderTargets(1, &aoRTV, aoDSV);
	aoMapPS->Apply();
	for (auto obj : mesh->objs)
	{
		XMMATRIX transf[4];
		transf[0] = obj->transform->WorldMatrix();
		transf[1] = CameraMgr::Instance()->Main()->VMat();
		transf[2] = projMat;
		transf[3] = XMMatrixTranspose(XMMatrixInverse(&XMMatrixDeterminant(transf[0]), transf[0]));

		aoMapVS->WriteCB(0, transf);
		aoMapVS->Apply();
		dsState->Apply();
		blendState->Apply();
		obj->RenderGeom();
	}
	graphic->RestoreViewport();
	graphic->RestoreRTV();
	// second-----------------------------------------------------------
	UINT stide = sizeof(Vertex_AO);
	UINT offset = 0;
	DX_DContext->IASetVertexBuffers(0, 1, &aoVB, &stide, &offset);
	DX_DContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	aoPS->WriteSRV(0, aoSRV);
	XMMATRIX projUv = projMat * XMMATRIX(
		0.5f, 0, 0, 0,
		0, -0.5f, 0, 0,
		0, 0, 1, 0,
		0.5f, 0.5f, 0, 1);
	aoPS->WriteCB(0, &projUv);
	aoPS->WriteCB(1, sample);
	aoVS->Apply();
	aoPS->Apply();
	noDsState->Apply();
	DX_DContext->Draw(4, 0);
	//-----------------------------------------------------------------------

	/*

	canvas->Update(timer->SPF());*/
}

void TestScene::Render(const Camera* camera, UINT sceneDepth)const
{
	/*DirectionalLight::Apply();
	PointLight::Apply();
	SpotLight::Apply();

	Scene::Render(camera, sceneDepth);

	canvas->Render();*/
}