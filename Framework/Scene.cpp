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
#include "CubeMap.h"
#include "Transform.h"

Scene::Scene(IGraphic* graphic)
	:graphic(graphic)
{
	camera = new Camera(FRAME_KIND_PERSPECTIVE, SCREEN_WIDTH, SCREEN_HEIGHT, 0.1, 1000, 1.1f, 1.0f, XMFLOAT3(0, 0, -100), FORWARD, RIGHT);
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
		XMFLOAT3(0, 0, 0), 200, XMFLOAT3(0.1f, 0.005f, 0.0005f)
	);
	pLight2 = new PointLight(
		XMFLOAT3(0.25f, 0,0),
		XMFLOAT3(0.7f, 0,0),
		XMFLOAT3(0.8f, 0,0),
		XMFLOAT3(0, 0,0), 200, XMFLOAT3(0.1f, 0.005f, 0.0005f)
	);

	std::vector<std::string> list;
	list.push_back("cm_normal_px.png");
	list.push_back("cm_normal_nx.png");
	list.push_back("cm_normal_py.png");
	list.push_back("cm_normal_ny.png");
	list.push_back("cm_normal_pz.png");
	list.push_back("cm_normal_nz.png");
	TextureMgr::Instance()->LoadCM("sky", list);
	TextureMgr::Instance()->Load("tex1", "woodbox.jpg", 4);
	ID3D11ShaderResourceView* sampleSRV;
	UINT sampleCount;
	TextureMgr::Instance()->Get("sky", &sampleSRV, &sampleCount);
	//canvas->Add("newAnim", XMFLOAT2(0, 50), 760, 320, 0, sampleSRV, sampleCount, 2);
	
	CubeMap* cm = new CubeMap(sampleSRV);
	objs.push_back(cm);

	ID3D11ShaderResourceView* texSRV;
	UINT texCount;
	TextureMgr::Instance()->Get("tex1", &texSRV, &texCount);
	Object* obj = new Object(new Sphere(4), XMFLOAT3(1, 1, 1), XMFLOAT3(1, 1, 1), XMFLOAT3(1, 1, 1), 4, XMFLOAT3(1, 1, 1), texSRV, sampleSRV, 2);
	obj->transform->SetScale(30, 50, 30);
	objs.push_back(obj);
}

Scene::~Scene()
{
	delete camera;
	delete canvas;
	delete dLight;
}

void Scene::Update()
{
	Timer::Update();
	camera->Update(Timer::SPF());
	/*
	if (Keyboard::IsPressing("F"))
	{
		p3.x -= 0.1f;
	}
	else if (Keyboard::IsPressing("H"))
	{
		p3.x += 0.1f;
	}
	if (Keyboard::IsPressing("T"))
	{
		p3.y += 0.1f;
	}
	else if (Keyboard::IsPressing("G"))
	{
		p3.y -= 0.1f;
	}*/
	

	if (pLight)
	{
		XMFLOAT3 pt = XMFLOAT3(
			cos(Timer::Elapsed()*0.5f) * 30,
			25,
			sin(Timer::Elapsed()*0.5f) * 50);
		pLight->SetPos(pt);
		Debugging::Instance()->Mark(999, pt, 1.5f, Colors::Green);
	}
	if (pLight2)
	{
		XMFLOAT3 pt = XMFLOAT3(
			cos(Timer::Elapsed() * 0.45f) * 35,
			cos(Timer::Elapsed()) * 10,
			sin(Timer::Elapsed() * 0.45f) * 55);
		pLight2->SetPos(pt);
		Debugging::Instance()->Mark(9999, pt, 1.5f, Colors::Red);
	}
	
	
	canvas->Update(Timer::SPF());

	for (auto obj : objs)
	{
		obj->Update(camera, XMMatrixIdentity());
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
