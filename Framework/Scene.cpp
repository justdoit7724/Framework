#include "Scene.h"
#include "Game_info.h"
#include "Camera.h"
#include "Keyboard.h"
#include "TextureMgr.h"
#include "Network.h"
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

Scene::Scene(IGraphic* graphic)
{
	ID3D11Device* device = graphic->Device();

	camera = new Camera(FRAME_KIND_PERSPECTIVE, SCREEN_WIDTH, SCREEN_HEIGHT ,0.1,200,1.1f, 1.0f, XMFLOAT3(0,0,-100), FORWARD, RIGHT);

	canvas = new UICanvas(device, SCREEN_WIDTH, SCREEN_HEIGHT);

	Debugging::EnableGrid(10,50);

	UINT spriteX=8;
	TextureMgr::Instance()->Load(graphic, "marine_s.png", spriteX);
	TextureMgr::Instance()->Load(graphic, "woodbox.jpg");
	TextureMgr::Instance()->Load(graphic, "heightmap3.jpg");

	ID3D11ShaderResourceView* srv=nullptr;
	TextureMgr::Instance()->Get("marine_s.png", &srv, &spriteX);
	
	canvas->Add(device, "Test", XMFLOAT2(0, 0), 200, 200, 1, srv, spriteX, 12);

	Sphere* tempQuad = new Sphere(device,3);
	ID3D11ShaderResourceView *const srv2 = TextureMgr::Instance()->Get("heightmap3.jpg");
	Hill* hill = new Hill(device, graphic->DContext(), 200, 200, XMFLOAT2(0, 30), &srv2);

	int objCountX = 4;
	int objCountY = 4;
	for (int z = 0; z < objCountY; ++z)
	{
		for (int x = 0; x < objCountX; ++x)
		{
			Object* newObj = new Object(
				graphic,
				tempQuad,
				XMFLOAT3(0.8f, 0.8f, 0.8f), XMFLOAT3(1, 1, 1), XMFLOAT3(0.9f, 0.9f, 0.9f), 8.0f, XMFLOAT3(1, 1, 1),
				"woodbox.jpg"
			);
			newObj->GetTransform()->SetScale(XMFLOAT3(4.0f, 4.0f, 4.0f));
			newObj->GetTransform()->SetTranslation(x*10,0,z*10);

			objs.push_back(newObj);

		}
	}
	Object* hillObj = new Object(
		graphic,
		hill,
		XMFLOAT3(0.8f, 0.8f, 0.8f), XMFLOAT3(1, 1, 1), XMFLOAT3(0.9f, 0.9f, 0.9f), 8.0f, XMFLOAT3(1, 1, 1),
		"grass.jpg"
	);
	hillObj->GetTransform()->SetScale(XMFLOAT3(14.0f, 1.0f, 14.0f));
	hillObj->GetTransform()->SetTranslation(10, 10, 0);
	objs.push_back(hillObj);

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

	XMMATRIX rot = XMMatrixRotationX(-Timer::Elapsed());
	(*(objs.rbegin()))->GetTransform()->SetRot(UP*rot, -FORWARD*rot);

	Debugging::Draw("Camera1 Pos", camera->Pos(), 10, 10);

}

void Scene::Render(IGraphic* graphic)
{
	Debugging::Render(camera, graphic);

	for (auto obj : objs)
	{
		obj->Render(graphic, camera, DirectionalLight::Data(), PointLight::Data(), SpotLight::Data(), XMMatrixIdentity());
	}

	canvas->Render(graphic);

}
