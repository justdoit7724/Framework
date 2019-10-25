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

TestScene::TestScene(IGraphic* graphic)
	:Scene("Test"),
	graphic(graphic)
{


	timer = new Timer();
	canvas = new UICanvas(SCREEN_WIDTH, SCREEN_HEIGHT);
	Debugging::Instance()->EnableGrid(10, 50);

	dLight = new DirectionalLight(
		XMFLOAT3(0.1f, 0.1f, 0.1f),
		XMFLOAT3(0.9f, 0.9f, 0.9f),
		XMFLOAT3(0.9f, 0.9f, 0.9f),
		XMFLOAT3(0.707f, -0.707f, 0));
	pLight = new PointLight(
		XMFLOAT3(0.1f, 0.1f, 0.1f),
		XMFLOAT3(0.9f, 0.9f, 0.9f),
		XMFLOAT3(0.8f, 0.8f, 0.8f),
		200, XMFLOAT3(0.03f, 0.005f, 0.001f), XMFLOAT3(0,0,0)
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
	
	nanosuit* badass = new nanosuit();
	badass->SetScale(XMFLOAT3(2, 2, 2));
	AddObj(badass);
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
		Debugging::Instance()->Mark(pt, 1.5f, Colors::WhiteSmoke);
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

	/*XMFLOAT3 moveScalar = XMFLOAT3(0, 0, 0);
	if (Keyboard::IsPressing("F")) {
		moveScalar += -RIGHT * 0.1f;
	}
	else if (Keyboard::IsPressing("H")) {

		moveScalar += RIGHT * 0.1f;
	}
	if (Keyboard::IsPressing("T")) {

		moveScalar += FORWARD * 0.1f;
	}
	else if (Keyboard::IsPressing("G")) {

		moveScalar += -FORWARD * 0.1f;
	}*/

	FrustumCulling(CameraMgr::Instance()->Main());
	Scene::Update(elapsed, spf);

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