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

#include "Shadow.h"
#include "Hill.h"
#include "Sphere.h"
#include "Quad.h"
#include "Cube.h"
#include "Shader.h"
#include "Light.h"
#include "Texture2D.h"
#include "Buffer.h"

//delete
Object* plane = nullptr; 
Shadow* shadow = nullptr;
XMFLOAT3 dLightDir = XMFLOAT3(0.707f,-0.707f,0);
std::vector<XMFLOAT3> vertice;

Scene::Scene()
{
	camera = new Camera(FRAME_KIND_PERSPECTIVE, SCREEN_WIDTH, SCREEN_HEIGHT, 0.1, 200, 1.1f, 1.0f, XMFLOAT3(0, 0, -100), FORWARD, RIGHT);
	canvas = new UICanvas(SCREEN_WIDTH, SCREEN_HEIGHT);
	Debugging::Instance()->EnableGrid(10, 50);

	dLight = new DirectionalLight(
		XMFLOAT3(0.25f, 0.25f, 0.25f),
		XMFLOAT3(0.7f, 0.7f, 0.7f),
		XMFLOAT3(0.8f, 0.8f, 0.8f),
		dLightDir);

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

	 plane = new Object(new Cube(), XMFLOAT3(1, 1, 1), XMFLOAT3(10, 10, 10), XMFLOAT3(1, 1, 1), 4, XMFLOAT3(1, 1, 1), "white.png");
	plane->GetTransform()->SetScale(20, 20, 20);
	plane->GetTransform()->SetTranslation(0, 30.0f, 0);
	XMMATRIX wMat = plane->GetTransform()->WorldMatrix();
	vertice.push_back(XMFLOAT3(-0.5f, -0.5f, -0.5f));
	vertice.push_back(XMFLOAT3(-0.5f,  0.5f, -0.5f));
	vertice.push_back(XMFLOAT3( 0.5f,  0.5f, -0.5f));
	vertice.push_back(XMFLOAT3( 0.5f, -0.5f, -0.5f));
	vertice.push_back(XMFLOAT3(-0.5f, -0.5f,  0.5f));
	vertice.push_back(XMFLOAT3(-0.5f,  0.5f,  0.5f));
	vertice.push_back(XMFLOAT3( 0.5f,  0.5f,  0.5f));
	vertice.push_back(XMFLOAT3( 0.5f, -0.5f,  0.5f));
	
	objs.push_back(plane);

	shadow = new Shadow(new Cube(), "white.png");
	shadow->GetTransform()->SetScale(20, 20, 20);
	shadow->GetTransform()->SetTranslation(0, 30.0f, 0);
	objs.push_back(shadow);
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

	if (Keyboard::IsPressing("F"))
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
	dLight->SetDir(dLightDir);

	XMMATRIX rotX = XMMatrixRotationX(Timer::Elapsed()*0.6f);
	XMMATRIX rotY = XMMatrixRotationY(Timer::Elapsed()*0.3f);
	plane->GetTransform()->SetRot(FORWARD*rotX*rotY, UP*rotX*rotY);
	shadow->GetTransform()->SetRot(FORWARD*rotX*rotY, UP*rotX*rotY);

	XMMATRIX objMat = plane->GetTransform()->WorldMatrix();
	for (int i=0; i<vertice.size(); ++i)
	{
		XMFLOAT3 pos = vertice[i] * objMat;
		Debugging::Instance()->DirLine(i, pos, dLightDir,Colors::Red);
		Debugging::Instance()->Mark(i, pos, 2);
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
