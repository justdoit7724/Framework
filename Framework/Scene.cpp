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

	Debugging::EnableGrid(10);

	//TextureMgr::Instance()->Load(graphic, "marine_s.png", 8);
	TextureMgr::Instance()->Load(graphic, "woodbox.jpg");

	/*UINT spriteX=0;
	ID3D11ShaderResourceView** srv=nullptr;
	TextureMgr::Instance()->Get("marine_s.png", srv, &spriteX);
	canvas->Add(device, "Test", XMFLOAT2(0, 0), 200, 200, 1, srv);
*/
	Sphere* tempQuad = new Sphere(device,3);

	int objCountX = 6;
	int objCountY = 4;
	for (int z = 0; z < objCountY; ++z)
	{
		for (int x = 0; x < objCountX; ++x)
		{
			Object* newObj = new Object(
				graphic,
				tempQuad,
				XMFLOAT3(0.8f, 0.8f, 0.8f), XMFLOAT3(0.15f, 0.15f, 0.15f), XMFLOAT3(0.9f, 0.9f, 0.9f), 8.0f, XMFLOAT3(1, 1, 1),
				"woodbox.jpg"
			);
			newObj->GetTransform()->SetScale(XMFLOAT3(1.0f, 1.0f, 1));
			newObj->GetTransform()->SetRot(UP, -FORWARD);
			newObj->GetTransform()->SetTranslation(x*10,0,z*10);

			objs.push_back(newObj);

		}
	}

	dLight = new DirectionalLight(
		XMFLOAT3(0.15f, 0.15f, 0.15f),
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

	Debugging::Draw("Camera1 Pos", camera->Pos(), 10, 10);

}

void Scene::Render(IGraphic* graphic)
{
	Debugging::Render(camera, graphic);

	canvas->Render(graphic);

	for (auto obj : objs)
	{
		obj->Render(graphic, camera, DirectionalLight::Data(), PointLight::Data(), SpotLight::Data(), XMMatrixIdentity());
	}
}
