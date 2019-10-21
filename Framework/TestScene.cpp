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

ShadowObj* cube;
ShadowObj* flor;
TestScene::TestScene(IGraphic* graphic)
	:Scene("Test"),
	graphic(graphic)
{
	timer = new Timer();
	canvas = new UICanvas(SCREEN_WIDTH, SCREEN_HEIGHT);
	Debugging::Instance()->EnableGrid(10, 50);

	/*dLight = new DirectionalLight(
		XMFLOAT3(0.1f, 0.1f, 0.1f),
		XMFLOAT3(0.9f, 0.9f, 0.9f),
		XMFLOAT3(0.9f, 0.9f, 0.9f),
		XMFLOAT3(0.707f, -0.707f, 0));*/
	pLight = new PointLight(
		XMFLOAT3(0.1f, 0.1f, 0.1f),
		XMFLOAT3(0.7f, 0.7f, 0.7f),
		XMFLOAT3(0.8f, 0.8f, 0.8f),
		200, XMFLOAT3(0.05f, 0.01f, 0.001f), XMFLOAT3(0,0,0)
	);
	/*pLight2 = new PointLight(
		XMFLOAT3(0.15f, 0.15f, 0.15f),
		XMFLOAT3(0.6f, 0.6f, 0.6f),
		XMFLOAT3(0.7f, 0.7f, 0.7f),
		XMFLOAT3(0, 0, 0), 200, XMFLOAT3(0.05f, 0.01f, 0.001f)
	);*/

	TextureMgr::Instance()->Load("rock", "rock.jpg", 10);
	TextureMgr::Instance()->Load("rock_normal", "rock_normal.jpg", 10);
	TextureMgr::Instance()->Load("ground", "ground.jpg", 10);
	TextureMgr::Instance()->Load("ground_normal", "ground_normal.jpg", 10);
	TextureMgr::Instance()->Load("rock_dp", "rock_dp.jpg", 10);
	TextureMgr::Instance()->Load("simple", "sample.jpg", 8);
	TextureMgr::Instance()->Load("white", "white.png", 8);
	TextureMgr::Instance()->Load("defaultNormal", "default_normal.png", 8);


	ID3D11ShaderResourceView* pbrSRV;
	ID3D11ShaderResourceView* pbrNormal;
	ID3D11ShaderResourceView* groundSRV;
	ID3D11ShaderResourceView* groundNormal;
	ID3D11ShaderResourceView* pbrDP;
	ID3D11ShaderResourceView* simpleSRV;
	ID3D11ShaderResourceView* whiteSRV;
	ID3D11ShaderResourceView* defaultNormal;
	TextureMgr::Instance()->Get("rock", &pbrSRV);
	TextureMgr::Instance()->Get("rock_normal", &pbrNormal);
	TextureMgr::Instance()->Get("ground", &groundSRV);
	TextureMgr::Instance()->Get("ground_normal", &groundNormal);
	TextureMgr::Instance()->Get("rock_dp", &pbrDP);
	TextureMgr::Instance()->Get("simple", &simpleSRV);
	TextureMgr::Instance()->Get("white", &whiteSRV);
	TextureMgr::Instance()->Get("defaultNormal", &defaultNormal);

	cube = new ShadowObj(new Cube(), pbrSRV, pbrNormal, 2);
	cube->transform->SetScale(15, 65, 15);
	cube->transform->SetTranslation(40, 40, 0);
	AddObj(cube);

	flor = new ShadowObj(new Quad(), pbrSRV, pbrNormal, 2);
	flor->transform->SetScale(300, 300, 1);
	flor->transform->SetTranslation(0, 1, 0);
	flor->transform->SetRot(UP, -FORWARD);
	AddObj(flor);


}

TestScene::~TestScene()
{
	delete canvas;
	delete dLight;
}

void TestScene::Logic_Update()
{
	pLight->Volume();
	timer->Update();

	float elaped = timer->Elapsed();
	if (dLight)
	{
		XMFLOAT3 pt = XMFLOAT3(
			150,
			150,
			0)*XMMatrixRotationY(elaped*0.1f);
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
		Debugging::Instance()->Mark(999, pt, 1.5f, Colors::WhiteSmoke);
	}
	if (pLight2)
	{
		XMFLOAT3 pt = XMFLOAT3(
			cos(elaped * 0.15f) * 15,
			cos(elaped) * 3,
			sin(elaped * 0.15f) * 25);
		pLight2->SetPos(pt);
		Debugging::Instance()->Mark(9999, pt, 1.5f, Colors::Red);
	}
}

void TestScene::Render_Update(const Camera* camera, float elapsed, float spf)
{
	pLight->ShadowCapture(objs);

	cube->Update(camera, elapsed, XMMatrixIdentity(), pLight->GetShadowPMat());
	cube->ps->WriteSRV(4, pLight->ShadowMapSRV());
	flor->Update(camera, elapsed, XMMatrixIdentity(), pLight->GetShadowPMat());
	flor->ps->WriteSRV(4, pLight->ShadowMapSRV());

	DirectionalLight::Apply();
	PointLight::Apply();
	SpotLight::Apply();

	canvas->Update(timer->SPF());
}

void TestScene::Render()const
{
	Scene::Render();

	canvas->Render();
}