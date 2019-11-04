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


void NDMapping(ID3D11RenderTargetView** rtv, ID3D11ShaderResourceView** srv, ID3D11DepthStencilView** dsv, D3D11_VIEWPORT& vp)
{
	D3D11_TEXTURE2D_DESC aoND_desc;
	aoND_desc.ArraySize = 1;
	aoND_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	aoND_desc.CPUAccessFlags = 0;
	aoND_desc.Format = DXGI_FORMAT_R32G32B32A32_TYPELESS;
	aoND_desc.Width = SCREEN_WIDTH * 0.5;
	aoND_desc.Height = SCREEN_HEIGHT * 0.5;
	aoND_desc.MipLevels = 1;
	aoND_desc.MiscFlags = 0;
	aoND_desc.SampleDesc = { 1,0 };
	aoND_desc.Usage = D3D11_USAGE_DEFAULT;
	ID3D11Texture2D* aoNDMap;
	r_assert(
		DX_Device->CreateTexture2D(&aoND_desc, nullptr, &aoNDMap)
	);
	D3D11_RENDER_TARGET_VIEW_DESC aoNDRTV_desc;
	aoNDRTV_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	aoNDRTV_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	aoNDRTV_desc.Texture2D.MipSlice = 0;
	r_assert(
		DX_Device->CreateRenderTargetView(aoNDMap, &aoNDRTV_desc, rtv)
	);
	D3D11_SHADER_RESOURCE_VIEW_DESC aoNDSRV_desc;
	aoNDSRV_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	aoNDSRV_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	aoNDSRV_desc.Texture2D.MipLevels = 1;
	aoNDSRV_desc.Texture2D.MostDetailedMip = 0;
	r_assert(
		DX_Device->CreateShaderResourceView(aoNDMap, &aoNDSRV_desc, srv)
	);
	D3D11_TEXTURE2D_DESC aoNDDepth_desc;
	aoNDDepth_desc.ArraySize = 1;
	aoNDDepth_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	aoNDDepth_desc.CPUAccessFlags = 0;
	aoNDDepth_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	aoNDDepth_desc.Width = aoND_desc.Width;
	aoNDDepth_desc.Height = aoND_desc.Height;
	aoNDDepth_desc.MipLevels = 1;
	aoNDDepth_desc.MiscFlags = 0;
	aoNDDepth_desc.SampleDesc = { 1,0 };
	aoNDDepth_desc.Usage = D3D11_USAGE_DEFAULT;
	ID3D11Texture2D* aoNDDepth;
	r_assert(
		DX_Device->CreateTexture2D(&aoNDDepth_desc, nullptr, &aoNDDepth)
	);
	D3D11_DEPTH_STENCIL_VIEW_DESC aoNDDSV_desc;
	aoNDDSV_desc.Flags = 0;
	aoNDDSV_desc.Format = aoNDDepth_desc.Format;
	aoNDDSV_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	aoNDDSV_desc.Texture2D.MipSlice = 0;
	r_assert(
		DX_Device->CreateDepthStencilView(aoNDDepth, &aoNDDSV_desc, dsv)
	);

	vp.Width = aoND_desc.Width;
	vp.Height = aoND_desc.Height;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
}
void ScreenMapping(ID3D11RenderTargetView** rtv, ID3D11ShaderResourceView** srv)
{
	D3D11_TEXTURE2D_DESC aoTex_desc;
	aoTex_desc.ArraySize = 1;
	aoTex_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	aoTex_desc.CPUAccessFlags = 0;
	aoTex_desc.Format = DXGI_FORMAT_R8G8B8A8_TYPELESS;
	aoTex_desc.Width = SCREEN_WIDTH;
	aoTex_desc.Height = SCREEN_HEIGHT;
	aoTex_desc.MipLevels = 1;
	aoTex_desc.MiscFlags = 0;
	aoTex_desc.Usage = D3D11_USAGE_DEFAULT;
	aoTex_desc.SampleDesc = { 1,0 };
	ID3D11Texture2D* aoTex;
	r_assert(
		DX_Device->CreateTexture2D(&aoTex_desc, nullptr, &aoTex)
	);
	D3D11_RENDER_TARGET_VIEW_DESC aoRTV_desc;
	aoRTV_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	aoRTV_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	aoRTV_desc.Texture2D.MipSlice = 0;
	r_assert(
		DX_Device->CreateRenderTargetView(aoTex, &aoRTV_desc, rtv)
	);
	D3D11_SHADER_RESOURCE_VIEW_DESC aoSRV_desc;
	aoSRV_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	aoSRV_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	aoSRV_desc.Texture2D.MipLevels = 1;
	aoSRV_desc.Texture2D.MostDetailedMip = 0;
	r_assert(
		DX_Device->CreateShaderResourceView(aoTex, &aoSRV_desc, srv)
	);
}
TestScene::TestScene(IGraphic* graphic)
	:Scene("Test"),
	graphic(graphic)
{
	timer = new Timer();
	canvas = new UICanvas(SCREEN_WIDTH, SCREEN_HEIGHT);

	/*dLight = new DirectionalLight(
		XMFLOAT3(0.4f, 0.4f, 0.4f),
		XMFLOAT3(0.7f, 0.7f, 0.7f),
		XMFLOAT3(0.7f, 0.7f, 0.7f),
		XMFLOAT3(0.707f, -0.707f, 0));*/
	/*pLight = new PointLight(
		XMFLOAT3(0.5f, 0.5f, 0.5f),
		XMFLOAT3(0.7f, 0.7f, 0.7f),
		XMFLOAT3(0.7f, 0.7f, 0.7f),
		200, XMFLOAT3(0.01f, 0.001f, 0.0005f), XMFLOAT3(0,0,0)
	);*/
	pLight2 = new PointLight(
		XMFLOAT3(0.15f, 0.15f, 0.15f),
		XMFLOAT3(0.6f, 0.6f, 0.6f),
		XMFLOAT3(0.7f, 0.7f, 0.7f),
		200, XMFLOAT3(0.01f, 0.001f, 0.0005f), XMFLOAT3(0, 0, 0)
	);

	TextureMgr::Instance()->Load("rock" ,"Data\\Texture\\rock.jpg");
	TextureMgr::Instance()->Load("rock_normal", "Data\\Texture\\rock_normal.jpg");
	TextureMgr::Instance()->Load("simple", "Data\\Texture\\sample.jpg");
	TextureMgr::Instance()->Load("simple2", "Data\\Texture\\sample2.jpg");
	TextureMgr::Instance()->Load("normal", "Data\\Texture\\default_normal.png");
	ID3D11ShaderResourceView* pbrSRV= TextureMgr::Instance()->Get("rock");
	ID3D11ShaderResourceView* pbrNormal= TextureMgr::Instance()->Get("rock_normal");
	ID3D11ShaderResourceView* simpleSRV= TextureMgr::Instance()->Get("simple");
	ID3D11ShaderResourceView* simpleSRV2= TextureMgr::Instance()->Get("simple2");
	ID3D11ShaderResourceView* defaultNormal = TextureMgr::Instance()->Get("normal");
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////


	NDMapping(&aoNDRTV, &aoNDSRV, &aoDSV, aoVp);
	ScreenMapping(&aoRTV, &aoSRV);
	ScreenMapping(&aoBlurRTV, &aoBlurSRV);
	ScreenMapping(&aoBlur2RTV, &aoBlur2SRV);

	canvas->Add("ssao", XMFLOAT2(0, 0), 300, 300, 0.5, aoBlur2SRV);

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
	D3D11_DEPTH_STENCIL_DESC equalDS_desc;
	ZeroMemory(&equalDS_desc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	equalDS_desc.DepthEnable = true;
	equalDS_desc.DepthFunc = D3D11_COMPARISON_EQUAL;
	equalDS_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	equalDsState = new DepthStencilState(&equalDS_desc);
	blendState = new BlendState(nullptr);
	aoBlurVS = new VShader("BlurAOVS.cso", aoILayout, ARRAYSIZE(aoILayout));
	aoBlurPS = new PShader("BlurAOPS.cso");
	aoBlurPS->AddSRV(0, 1);
	aoBlurPS->AddSRV(1, 1);
	aoBlurPS->AddCB(0, 1, sizeof(float));
	aoBlurPS->AddSamp(0, 1, &aoSamp_desc);

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

	
	const int N = 3;
	for (int z = 0; z < N; ++z) {
		for (int y = 0; y < N; ++y) {
			for (int x = 0; x < N; ++x) {
				float interval = 12.5;
				XMFLOAT3 scale = XMFLOAT3(10, 10, 10);
				XMFLOAT3 offset = XMFLOAT3(scale.x,0,scale.z)* (N / 2);
				Object* cube = new Object(new Cube(), simpleSRV2, defaultNormal);
				cube->transform->SetScale(scale);
				cube->transform->SetTranslation(XMFLOAT3(x * interval,5.0f + y * interval, z * interval)-offset);
				AddObj(cube);
			}
		}
	}
	flor = new Object(new Quad(), simpleSRV, defaultNormal);
	flor->transform->SetScale(100, 100, 1);
	flor->transform->SetRot(UP, FORWARD);
	AddObj(flor);

	mesh = new nanosuit();
	mesh->SetScale(XMFLOAT3(3, 3, 3));
	for (auto obj : mesh->objs)
	{
		//AddObj(obj);
	}

	//Debugging::Instance()->EnableGrid(10);
}

TestScene::~TestScene()
{
	delete canvas;
	delete dLight;
}


void TestScene::Update(float elapsed, float spf)
{
	if(pLight)
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
			0.5f,
			sin(elaped * 0.6f) * 20);
		pLight->SetPos(pt);
		//Debugging::Instance()->Mark(pt, 1.5f, Colors::WhiteSmoke);
	}
	if (pLight2)
	{
		XMFLOAT3 pt = XMFLOAT3(
			cos(elaped * 0.15f) * 15,
			cos(elaped*0.1f) * 2,
			sin(elaped * 0.15f) * 25);
		pLight2->SetPos(pt);
		//Debugging::Instance()->Mark(pt, 1.5f, Colors::Red);
		//Debugging::Instance()->Draw("Pos = ", pt, 10, 10);
	}

	Scene::Update(elaped, spf);
	FrustumCulling(CameraMgr::Instance()->Main());

	// first -------------------------------------------------------------
	XMMATRIX projMat= CameraMgr::Instance()->Main()->ProjMat(Z_ORDER_STANDARD);
	const float defaultColor[4] = { 1,1,1,100000 };
	DX_DContext->RSSetViewports(1, &aoVp);
	ID3D11ShaderResourceView* nullSRV = nullptr;
	DX_DContext->PSSetShaderResources(0, 1, &nullSRV);
	DX_DContext->PSSetShaderResources(1, 1, &nullSRV);
	DX_DContext->ClearRenderTargetView(aoNDRTV, defaultColor);
	DX_DContext->ClearDepthStencilView(aoDSV, D3D11_CLEAR_DEPTH, 1.0, NULL);
	DX_DContext->OMSetRenderTargets(1, &aoNDRTV, aoDSV);
	aoMapPS->Apply();
	for (auto obj :drawObjs)
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
	// second-----------------------------------------------------------
	graphic->RestoreViewport();
	DX_DContext->PSSetShaderResources(0, 1, &nullSRV);
	DX_DContext->PSSetShaderResources(1, 1, &nullSRV);
	DX_DContext->ClearRenderTargetView(aoRTV, defaultColor);
	DX_DContext->OMSetRenderTargets(1, &aoRTV, nullptr);
	UINT stide = sizeof(Vertex_AO);
	UINT offset = 0;
	DX_DContext->IASetVertexBuffers(0, 1, &aoVB, &stide, &offset);
	DX_DContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	aoPS->WriteSRV(0, aoNDSRV);
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
	// First Blur -----------------------------------------------------------------
	DX_DContext->ClearRenderTargetView(aoBlurRTV, defaultColor);
	DX_DContext->PSSetShaderResources(0, 1, &nullSRV);
	DX_DContext->PSSetShaderResources(1, 1, &nullSRV);
	DX_DContext->OMSetRenderTargets(1, &aoBlurRTV, nullptr);
	aoBlurVS->Apply();
	aoBlurPS->WriteSRV(0, aoNDSRV);
	aoBlurPS->WriteSRV(1, aoSRV);
	float curDirection = 1;
	aoBlurPS->WriteCB(0, &curDirection);
	aoBlurPS->Apply();
	noDsState->Apply();
	DX_DContext->Draw(4, 0);
	// Second blur -----------------------------------------------------------------------
	DX_DContext->ClearRenderTargetView(aoBlur2RTV, defaultColor);
	DX_DContext->PSSetShaderResources(3, 1, &nullSRV);
	DX_DContext->OMSetRenderTargets(1, &aoBlur2RTV, nullptr);
	aoBlurVS->Apply();
	aoBlurPS->WriteSRV(0, aoNDSRV);
	aoBlurPS->WriteSRV(1, aoBlurSRV);
	curDirection = 0;
	aoBlurPS->WriteCB(0, &curDirection);
	aoBlurPS->Apply();
	noDsState->Apply();
	DX_DContext->Draw(4, 0);
	//////////////////////////////////////////////////////////////////////
	ID3D11RenderTargetView* defaultRTV = /*graphic->RenderTargetView()*/nullptr;
	DX_DContext->OMSetRenderTargets(1, &defaultRTV, aoDSV);
	graphic->RestoreRTV();

	DX_DContext->PSSetShaderResources(3, 1, &aoBlur2SRV);

	canvas->Update(timer->SPF());
}

void TestScene::Render(const Camera* camera, UINT sceneDepth)const
{
	DirectionalLight::Apply();
	PointLight::Apply();
	SpotLight::Apply();

	Scene::Render(camera, sceneDepth);

	canvas->Render();
}