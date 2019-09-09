#include "Scene.h"
#include "Game_info.h"
#include "Camera.h"
#include "Keyboard.h"
#include "TextureMgr.h"
#include "Camera.h"
#include "Timer.h"
#include "CustomFormat.h"
#include "UI.h"
#include "Object.h"
#include "Geometrics.h"
#include "DepthStencilState.h"
#include "BlendState.h"""

#include "Hill.h"
#include "Sphere.h"
#include "Quad.h"
#include "Cube.h"
#include "Shader.h"
#include "Light.h"
#include "Texture2D.h"
#include "Buffer.h"

Scene::Scene()
{
	camera = new Camera(FRAME_KIND_PERSPECTIVE, SCREEN_WIDTH, SCREEN_HEIGHT ,0.1,200,1.1f, 1.0f, XMFLOAT3(0,0,-100), FORWARD, RIGHT);

	canvas = new UICanvas(SCREEN_WIDTH, SCREEN_HEIGHT);

	Debugging::EnableGrid(10,50);

	TextureMgr::Instance()->Load("sample.jpg");
	TextureMgr::Instance()->Load("transparency.png");
	TextureMgr::Instance()->Load("marine_s.png");
	TextureMgr::Instance()->Load("heightmap3.jpg");
	TextureMgr::Instance()->Load("grass.jpg");

	canvas->Add("test", XMFLOAT2(380, 380), 380, 380, 0, TextureMgr::Instance()->Get("sample.jpg"));
	D3D11_DEPTH_STENCIL_DESC ds_desc;
	ds_desc.DepthEnable = true;
	ds_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	ds_desc.DepthFunc = D3D11_COMPARISON_LESS;
	ds_desc.StencilEnable = true;
	ds_desc.StencilReadMask = 0xff;
	ds_desc.StencilWriteMask = 0xff;
	D3D11_DEPTH_STENCILOP_DESC dso_desc;
	dso_desc.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	dso_desc.StencilFailOp = D3D11_STENCIL_OP_REPLACE;
	dso_desc.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dso_desc.StencilFunc = D3D11_COMPARISON_ALWAYS;
	

	Hill* hill = new Hill(400, 400, XMFLOAT2(0, 30), TextureMgr::Instance()->GetAddress("heightmap3.jpg"));
	Hill* water = new Hill(50, 50, XMFLOAT2(10, 15), TextureMgr::Instance()->GetAddress("sample.jpg"));

	Object* hillObj = new Object(hill, XMFLOAT3(1, 1, 1), XMFLOAT3(1, 1, 1), XMFLOAT3(0.5f, 0.5f, 0.5f), 4, XMFLOAT3(1, 1, 1), "grass.jpg");
	Object* waterObj = new Object(water, XMFLOAT3(1, 1, 1), XMFLOAT3(1, 1, 1), XMFLOAT3(1, 1, 1), 8, XMFLOAT3(1, 1, 1), "white.png");
	waterObj->SetTransparency(0.5f);
	D3D11_BLEND_DESC tDesc;
	tDesc.AlphaToCoverageEnable = false;
	tDesc.IndependentBlendEnable = false;
	tDesc.RenderTarget[0].BlendEnable = true;
	tDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	tDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	tDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	tDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	tDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	tDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	tDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	waterObj->GetBlendState()->Modify(device, &tDesc);

	hillObj->GetTransform()->SetScale(100, 1, 100);
	waterObj->GetTransform()->SetScale(100, 1, 100);
	objs.push_back(hillObj);
	objs.push_back(waterObj);

	dLight = new DirectionalLight(
		XMFLOAT3(0.25f, 0.25f, 0.25f),
		XMFLOAT3(0.75f, 0.75f, 0.75f),
		XMFLOAT3(0.9f, 0.9f, 0.9f),
		FORWARD);
}

Scene::~Scene()
{
	
}

//static XMMATRIX
void Scene::Update()
{
	Timer::Update();
	camera->Update(Timer::SPF());
	canvas->Update(Timer::SPF());

	for (auto obj : objs)
	{
		obj->Update(camera, DirectionalLight::Data(), PointLight::Data(), SpotLight::Data(), XMMatrixIdentity());
	}
}

void Scene::Render()
{
	Debugging::Render(camera);

	for (auto obj : objs)
	{
		obj->Render();
	}

	canvas->Render();

}
