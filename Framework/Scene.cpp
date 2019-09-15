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

#include "Hill.h"
#include "Sphere.h"
#include "Quad.h"
#include "Cube.h"
#include "Shader.h"
#include "Light.h"
#include "Texture2D.h"
#include "Buffer.h"

//delete
Object* obj = nullptr; 
XMFLOAT3 dLightDir = XMFLOAT3(0.707f,-0.707f,0);
DirectionalLight* dLight2;
PointLight* pLight;
PointLight* pLight2;
std::vector<XMFLOAT3> dirs;

Scene::Scene()
{
	camera = new Camera(FRAME_KIND_PERSPECTIVE, SCREEN_WIDTH, SCREEN_HEIGHT, 0.1, 200, 1.1f, 1.0f, XMFLOAT3(0, 0, -100), FORWARD, RIGHT);
	canvas = new UICanvas(SCREEN_WIDTH, SCREEN_HEIGHT);
	Debugging::Instance()->EnableGrid(10, 50);

	/*dLight = new DirectionalLight(
		XMFLOAT3(0.25f, 0.25f, 0.25f),
		XMFLOAT3(0.7f, 0.7f, 0.7f),
		XMFLOAT3(0.8f, 0.8f, 0.8f),
		dLightDir);
	dLight2 = new DirectionalLight(
		XMFLOAT3(0.25f, 0, 0),
		XMFLOAT3(0.7f, 0, 0),
		XMFLOAT3(0.8f, 0, 0),
		XMFLOAT3(-0.707f, -0.707f, 0)
	);*/
	pLight = new PointLight(
		XMFLOAT3(0.55f, 0.55f, 0.55f),
		XMFLOAT3(0.7f, 0.7f, 0.7f),
		XMFLOAT3(0, 0.8f, 0),
		XMFLOAT3(0, 60, 0), 300, XMFLOAT3(1, 0.1f, 0.01f)
	);
	/*pLight2 = new PointLight(
		XMFLOAT3(0, 0, 0.25f),
		XMFLOAT3(0, 0, 0.7f),
		XMFLOAT3(0, 0, 0.8f),
		XMFLOAT3(40, 60, 0), 100, XMFLOAT3(1, 0.1f, 0.01f)
	);*/

	TextureMgr::Instance()->Load("sample.jpg");
	TextureMgr::Instance()->Load("white.png");
	TextureMgr::Instance()->Load("black.png");
	TextureMgr::Instance()->Load("marine_s.png");
	TextureMgr::Instance()->Load("heightmap3.jpg");
	TextureMgr::Instance()->Load("woodbox.jpg");

	Object* floor = new Object(new Quad(), XMFLOAT3(1, 1, 1), XMFLOAT3(1, 1, 1), XMFLOAT3(1, 1, 1), 4, XMFLOAT3(1, 1, 1), "sample.jpg");
	floor->GetTransform()->SetScale(200, 200, 1);
	floor->GetTransform()->SetRot(UP,FORWARD);
	floor->GetTransform()->SetTranslation(0, 0.01f, 0);
	objs.push_back(floor);

	 obj = new Object(new Cube(), XMFLOAT3(1, 1, 1), XMFLOAT3(1, 1, 1), XMFLOAT3(1, 1, 1), 4, XMFLOAT3(1, 1, 1), "woodbox.jpg");
	obj->GetTransform()->SetScale(20, 20, 20);
	obj->GetTransform()->SetTranslation(0, 30.0f, 0);
	obj->EnableShadow(UP, 0.2f, 0.5f);
	dirs.push_back(XMFLOAT3(-0.5,-0.5,-0.5));
	dirs.push_back(XMFLOAT3(-0.5, 0.5,-0.5));
	dirs.push_back(XMFLOAT3( 0.5, 0.5,-0.5));
	dirs.push_back(XMFLOAT3( 0.5,-0.5,-0.5));
	dirs.push_back(XMFLOAT3(-0.5,-0.5, 0.5));
	dirs.push_back(XMFLOAT3(-0.5, 0.5, 0.5));
	dirs.push_back(XMFLOAT3( 0.5, 0.5, 0.5));
	dirs.push_back(XMFLOAT3( 0.5,-0.5, 0.5));

	objs.push_back(obj);
}

XMFLOAT3 pos = XMFLOAT3(0, 100, 0);

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
	canvas->Update(Timer::SPF());

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

	XMMATRIX rotX = XMMatrixRotationX(Timer::Elapsed()*0.6f);
	XMMATRIX rotY = XMMatrixRotationY(Timer::Elapsed()*0.3f);
	obj->GetTransform()->SetRot(FORWARD*rotX*rotY, UP*rotX*rotY);

	XMFLOAT3 pt = XMFLOAT3(
		cos(Timer::Elapsed()*0.5f) * 50,
		45,
		sin(Timer::Elapsed()*0.5f) * 100);
	pLight->SetPos(pt);
	Debugging::Instance()->Mark(0, pt, 4);

	XMMATRIX wMat = obj->GetTransform()->WorldMatrix();
	for (int i=0; i<dirs.size(); ++i)
	{
		XMFLOAT3 pos = dirs[i] * wMat;
		Debugging::Instance()->Mark(i, pos, 2);
		Debugging::Instance()->DirLine(i, pLight->GetTransform()->GetPos(), Normalize(dirs[i] * wMat - pLight->GetTransform()->GetPos()));
	}

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
