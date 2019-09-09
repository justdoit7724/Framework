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

	TextureMgr::Instance()->Load("sample.jpg");
	TextureMgr::Instance()->Load("white.png");
	TextureMgr::Instance()->Load("marine_s.png");
	TextureMgr::Instance()->Load("heightmap3.jpg");
	TextureMgr::Instance()->Load("woodbox.jpg");

	canvas = new UICanvas(SCREEN_WIDTH, SCREEN_HEIGHT);
	canvas->Add("TestUI", XMFLOAT2(30, 30), 100, 100, 0, TextureMgr::Instance()->Get("sample.jpg"));

	Debugging::Instance()->EnableGrid(10,50);

	Object* mask = new Object(new Quad(), XMFLOAT3(1, 1, 1), XMFLOAT3(1, 1, 1), XMFLOAT3(1, 1, 1), 8, XMFLOAT3(1, 1, 1), "white.png");
	Object* obj = new Object(new Cube(), XMFLOAT3(1, 1, 1), XMFLOAT3(1, 1, 1), XMFLOAT3(1, 1, 1), 8, XMFLOAT3(1, 1, 1), "sample.jpg");
	Object* obj2 = new Object(new Cube(), XMFLOAT3(1, 1, 1), XMFLOAT3(1, 1, 1), XMFLOAT3(1, 1, 1), 8, XMFLOAT3(1, 1, 1), "sample.jpg");

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
	mask->GetBlendState()->Modify(&tDesc);
	D3D11_DEPTH_STENCIL_DESC maskDesc;
	maskDesc.DepthEnable = true;
	maskDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	maskDesc.DepthFunc = D3D11_COMPARISON_LESS;
	maskDesc.StencilEnable = true;
	maskDesc.StencilReadMask = 0xff;
	maskDesc.StencilWriteMask = 0xff;
	D3D11_DEPTH_STENCILOP_DESC dsoDesc;
	dsoDesc.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	dsoDesc.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsoDesc.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dsoDesc.StencilFunc = D3D11_COMPARISON_ALWAYS;
	maskDesc.FrontFace = dsoDesc;
	maskDesc.BackFace = dsoDesc;
	mask->GetDepthStencilState()->Modify(&maskDesc);
	mask->GetDepthStencilState()->SetRefValue(1);
	D3D11_DEPTH_STENCIL_DESC normalDesc;
	normalDesc.DepthEnable = true;
	normalDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	normalDesc.DepthFunc = D3D11_COMPARISON_LESS;
	normalDesc.StencilEnable = true;
	normalDesc.StencilReadMask = 0xff;
	normalDesc.StencilWriteMask = 0xff;
	dsoDesc.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsoDesc.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsoDesc.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dsoDesc.StencilFunc = D3D11_COMPARISON_EQUAL;
	normalDesc.FrontFace = dsoDesc;
	normalDesc.BackFace = dsoDesc;
	obj->GetDepthStencilState()->Modify(&normalDesc);
	obj->GetDepthStencilState()->SetRefValue(1);

	mask->SetTransparency(0.3f);

	obj->GetTransform()->SetScale(30, 30, 25);
	obj->GetTransform()->SetTranslation(0, 0, 50);
	obj2->GetTransform()->SetScale(30, 30, 25);
	obj2->GetTransform()->SetTranslation(0, 0, -50);
	mask->GetTransform()->SetScale(75, 75, 1);
	mask->GetTransform()->SetRot(-FORWARD, UP);
	objs.push_back(mask);
	objs.push_back(obj);
	objs.push_back(obj2);

	dLight = new DirectionalLight(
		XMFLOAT3(0.25f, 0.25f, 0.25f),
		XMFLOAT3(0.7f, 0.7f, 0.7f),
		XMFLOAT3(0.8f, 0.8f, 0.8f),
		FORWARD);
}

Scene::~Scene()
{
	
}

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
	Debugging::Instance()->Render(camera);

	for (auto obj : objs)
	{
		obj->Render();
	}

	canvas->Render();

}
