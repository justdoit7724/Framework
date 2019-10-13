#include "TestScene.h"
#include "Game_info.h"
#include "Camera.h"
#include "Keyboard.h"
#include "Mouse.h"	
#include "TextureMgr.h"
#include "Camera.h"
#include "CustomFormat.h"
#include "UI.h"
#include "Object.h"
#include "Geometrics.h"
#include "Network.h"

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

//debug -- remove
ShadowMap* shadowMap;
Camera* dLightCamera;
ShadowObj* cube;
ShadowObj* flor;
TestScene::TestScene(IGraphic* graphic)
	:Scene("Test"),
	graphic(graphic)
{
	
	Camera* camera = new Camera("TestMain", FRAME_KIND_PERSPECTIVE, SCREEN_WIDTH, SCREEN_HEIGHT, 0.1, 1000, 1.1f, 1.0f, XMFLOAT3(0, 0, -100), FORWARD, RIGHT);
	camera->SetMain();

	timer = new Timer();
	canvas = new UICanvas(SCREEN_WIDTH, SCREEN_HEIGHT);
	Debugging::Instance()->EnableGrid(10, 50);

	dLight = new DirectionalLight(
		XMFLOAT3(0.1f, 0.1f, 0.1f),
		XMFLOAT3(0.9f, 0.9f, 0.9f),
		XMFLOAT3(0.9f, 0.9f, 0.9f),
		XMFLOAT3(0.707f, -0.707f, 0));
	/*pLight = new PointLight(
		XMFLOAT3(0.1f, 0.1f, 0.1f),
		XMFLOAT3(0.7f, 0.7f, 0.7f),
		XMFLOAT3(0.8f, 0.8f, 0.8f),
		XMFLOAT3(0, 0, 0), 200, XMFLOAT3(0.05f, 0.01f, 0.001f)
	);*/
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

	/*Camera* pt_cam = new Camera("test", FRAME_KIND_ORTHOGONAL, SCREEN_WIDTH, SCREEN_HEIGHT, 0.1f, 1000.0f, 90.0f, 1, XMFLOAT3(-300, 300, 0), Normalize(XMFLOAT3(0.707, -0.707, 0)), Normalize(XMFLOAT3(0.707, 0.707, 0)));
	Debugging::Instance()->Mark(3, pt_cam->transform->GetPos());
	PT_Obj* pt = new PT_Obj(new Quad(), pt_cam->VPMat(2), XMFLOAT3(1, 1, 1), 1, XMFLOAT3(1, 1, 1), XMFLOAT3(1, 1, 1), 4, XMFLOAT3(0, 0, 0), pbrSRV, pbrNormal, simpleSRV, nullptr, 2);
	pt->transform->SetScale(1600, 1000, 1);
	pt->transform->SetRot(UP, -FORWARD);
	delete pt_cam;*/

	for (int i = 0; i < 10; ++i)
	{
		Object* newObj = new Object(new Quad(), XMFLOAT3(1, 1, 1), XMFLOAT3(1, 1, 1), XMFLOAT3(1, 1, 1), 4, XMFLOAT3(1, 1, 1), whiteSRV, defaultNormal, nullptr, 2);
		newObj->transform->SetScale(40, 60, 1);
		float angle10 = XM_PI / 18.0;
		XMMATRIX rot = XMMatrixRotationX((i - 5) * angle10);
		newObj->transform->SetRot(-FORWARD* rot, UP* rot, -RIGHT);
		newObj->transform->SetTranslation(-350+i*70,0,0);

		AddObj(newObj);
	}

	cube = new ShadowObj(new Cube(), whiteSRV, defaultNormal, Z_ORDER_STANDARD);
	cube->transform->SetScale(20, 40, 20);
	cube->transform->SetTranslation(0, 40, 0);
	flor = new ShadowObj(new Quad(), whiteSRV, defaultNormal, Z_ORDER_STANDARD);
	flor->transform->SetRot(UP, -FORWARD, Cross(-FORWARD,UP));
	flor->transform->SetScale(600, 600, 1);

	shadowMap = new ShadowMap(SCREEN_WIDTH, SCREEN_HEIGHT);
	dLightCamera = new Camera("Test", FRAME_KIND_ORTHOGONAL, SCREEN_WIDTH, SCREEN_HEIGHT, 0.1f, 300.0f, 90.0f, 1.0f, XMFLOAT3(0, 0, -100), FORWARD, UP);
	
	
}

TestScene::~TestScene()
{
	delete canvas;
	delete dLight;
}

void CameraMove(Camera* camera, float spf) {

	XMFLOAT3 newPos = camera->transform->GetPos();
	XMFLOAT3 right = camera->transform->GetRight();
	XMFLOAT3 forward = camera->transform->GetForward();
	const float speed = 100;
	if (Keyboard::IsPressing('A')) {

		newPos += -right * speed * spf;
	}
	else if (Keyboard::IsPressing('D')) {

		newPos += right * speed * spf;
	}
	if (Keyboard::IsPressing('S')) {

		newPos += -forward * speed * spf;
	}
	else if (Keyboard::IsPressing('W')) {

		newPos += forward * speed * spf;
	}
	static float angleX = 0;
	static float angleY = 0;
	static XMFLOAT2 prevMousePt;
	const float angleSpeed = 3.141592f * 0.2f;
	if (Mouse::Instance()->IsRightDown())
	{
		angleY += angleSpeed * spf * (Mouse::Instance()->X() - prevMousePt.x);
		angleX += angleSpeed * spf * (Mouse::Instance()->Y() - prevMousePt.y);
	}
	prevMousePt.x = Mouse::Instance()->X();
	prevMousePt.y = Mouse::Instance()->Y();
	const XMMATRIX rotMat = XMMatrixRotationX(angleX) * XMMatrixRotationY(angleY);
	camera->transform->SetTranslation(newPos);
	XMFLOAT3 f = FORWARD * rotMat;
	XMFLOAT3 u = UP * rotMat;
	camera->transform->SetRot(f,u,Cross(u,f));
}
void TestScene::Logic_Update()
{
	Camera* cam = CameraMgr::Instance()->Main();

	timer->Update();

	CameraMove(cam, timer->SPF());

	Debugging::Instance()->Draw("Main camera = ", cam->transform->GetPos(), 10, 40);

	float elaped = timer->Elapsed();
	if (dLight)
	{
		XMFLOAT3 pt = XMFLOAT3(
			100,
			100,
			0)*XMMatrixRotationY(elaped*0.4f);
		XMFLOAT3 dir = Normalize(XMFLOAT3(-pt.x, -pt.y, -pt.z));
		dLight->transform->SetTranslation(pt);
		dLightCamera->transform->SetTranslation(pt);
		dLight->SetDir(dir);
		dLightCamera->transform->SetRot(dLight->transform->GetForward(), dLight->transform->GetUp(), dLight->transform->GetRight());

		Debugging::Instance()->Mark(3453, pt, 4);
		Debugging::Instance()->DirLine(523, pt, dir, 15);
	}
	if (pLight)
	{
		XMFLOAT3 pt = XMFLOAT3(
			cos(elaped * 0.3f) * 10,
			10,
			sin(elaped * 0.3f) * 10);
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

	/*XMMATRIX rotMat = XMMatrixRotationY(elaped * 0.4f);
	quad->transform->SetRot(UP, -FORWARD*rotMat);*/
	

	/*XMMATRIX rot = XMMatrixRotationX(elaped * 0.2f) * XMMatrixRotationY(elaped*0.15f);
	sphere->transform->SetRot(FORWARD * rot, UP * rot);
	cylinder->transform->SetRot(FORWARD * rot, UP * rot);*/
}


void TestScene::Render_Update(const Camera* camera, float elapsed)
{

	const XMMATRIX shadowVP = dLightCamera->VPMat(2);
	ID3D11ShaderResourceView* nullSRV = nullptr;
	DX_DContext->PSSetShaderResources(3, 1, &nullSRV);
	std::vector<Object*> shadowObjs;
	shadowObjs.push_back(flor);
	shadowObjs.push_back(cube);
	shadowMap->Mapping(shadowObjs, shadowVP);
	graphic->RestoreRTV();
	graphic->RestoreViewport();

	cube->Update(camera, elapsed, shadowVP);
	cube->ps->WriteSRV(3, shadowMap->Depth());
	flor->Update(camera, elapsed, shadowVP);
	flor->ps->WriteSRV(3, shadowMap->Depth());

	//debug - decomment
	//Scene::Render_Update(camera, elapsed);

	canvas->Update(timer->SPF());
}

void TestScene::Render()const
{
	//debug - remove
	cube->Render();
	flor->Render();

	//debug - decomment
	//Scene::Render();

	canvas->Render();
}
