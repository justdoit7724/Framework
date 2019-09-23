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
#include "Network.h"

#include "Hill.h"
#include "Sphere.h"
#include "Quad.h"
#include "Cube.h"
#include "Shader.h"
#include "Light.h"
#include "Texture2D.h"
#include "Buffer.h"

Scene::Scene(IGraphic* graphic)
	:graphic(graphic)
{
	camera = new Camera(FRAME_KIND_PERSPECTIVE, SCREEN_WIDTH, SCREEN_HEIGHT, 0.1, 200, 1.1f, 1.0f, XMFLOAT3(0, 0, -100), FORWARD, RIGHT);
	canvas = new UICanvas(SCREEN_WIDTH, SCREEN_HEIGHT);
	Debugging::Instance()->EnableGrid(10, 50);

	dLight = new DirectionalLight(
		XMFLOAT3(0.25f, 0.25f, 0.25f),
		XMFLOAT3(0.7f, 0.7f, 0.7f),
		XMFLOAT3(0.8f, 0.8f, 0.8f),
		XMFLOAT3(0.707f, -0.707f,0));
	pLight = new PointLight(
		XMFLOAT3(0, 0.25f, 0),
		XMFLOAT3(0, 0.7f, 0),
		XMFLOAT3(0, 0.8f, 0),
		XMFLOAT3(0, 0, 0), 100, XMFLOAT3(0.15f, 0.015f, 0.0015f)
	);

	std::vector<std::string> list;
	list.push_back("marine_s.png");
	TextureMgr::Instance()->Load("star", list, 10);
	ID3D11ShaderResourceView* sampleSRV;
	UINT sampleCount;
	TextureMgr::Instance()->Get("star", &sampleSRV, &sampleCount);
	canvas->Add("newAnim", XMFLOAT2(0, 50), 1680, 360, 0, sampleSRV, sampleCount, 2);
	D3D11_BLEND_DESC uiBlend_desc;
	uiBlend_desc.AlphaToCoverageEnable = true;
	uiBlend_desc.IndependentBlendEnable = false;
	uiBlend_desc.RenderTarget[0].BlendEnable = false;
	uiBlend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	canvas->Get("newAnim")->blendState->Modify(&uiBlend_desc);
}

Scene::~Scene()
{
	delete camera;
	delete canvas;
	delete dLight;
}


ID3D11ShaderResourceView* depthComplexSrv;
void Scene::Update()
{
	Timer::Update();
	camera->Update(Timer::SPF());

	/*if (Keyboard::IsPressing("F"))
	{
		dLightDir = dLightDir*XMMatrixRotationZ(0.01f);
	}
	else if (Keyboard::IsPressing("H"))
	{
		dLightDir = dLightDir * XMMatrixRotationZ(-0.01f);

	}
	if (Keyboard::IsPressing("T"))
	{
		dLightDir = dLightDir * XMMatrixRotationX(0.01f);
	}
	else if (Keyboard::IsPressing("G"))
	{
		dLightDir = dLightDir * XMMatrixRotationX(-0.01f);
	}
	dLight->SetDir(dLightDir);*/

	if (pLight)
	{
		XMFLOAT3 pt = XMFLOAT3(
			cos(Timer::Elapsed()*0.5f) * 30,
			25,
			sin(Timer::Elapsed()*0.5f) * 50);
		pLight->SetPos(pt);
		Debugging::Instance()->Mark(0, pt, 1.5f, Colors::Green);
	}

	
	
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
