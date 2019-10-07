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

TestScene::TestScene(IGraphic* graphic)
	:Scene("Test")
{
	Camera* camera = new Camera("TestMain", FRAME_KIND_PERSPECTIVE, SCREEN_WIDTH, SCREEN_HEIGHT, 0.1, 1000, 1.1f, 1.0f, XMFLOAT3(0, 0, -100), FORWARD, RIGHT);
	camera->SetMain();
	timer = new Timer();
	canvas = new UICanvas(SCREEN_WIDTH, SCREEN_HEIGHT);
	Debugging::Instance()->EnableGrid(10, 50);

	/*dLight = new DirectionalLight(
		XMFLOAT3(0.1f, 0.1f, 0.1f),
		XMFLOAT3(0.25f, 0.25f, 0.25f),
		XMFLOAT3(0.5f, 0.5f, 0.5f),
		XMFLOAT3(0.707f, -0.707f, 0));*/
	pLight = new PointLight(
		XMFLOAT3(0.1f, 0.1f, 0.1f),
		XMFLOAT3(0.7f, 0.7f, 0.7f),
		XMFLOAT3(0.8f, 0.8f, 0.8f),
		XMFLOAT3(0, 0, 0), 200, XMFLOAT3(0.05f, 0.01f, 0.001f)
	);
	/*pLight2 = new PointLight(
		XMFLOAT3(0.15f, 0.15f, 0.15f),
		XMFLOAT3(0.6f, 0.6f, 0.6f),
		XMFLOAT3(0.7f, 0.7f, 0.7f),
		XMFLOAT3(0, 0, 0), 200, XMFLOAT3(0.05f, 0.01f, 0.001f)
	);*/

	std::vector<std::string> list;
	list.push_back("cm_normal_px.png");
	list.push_back("cm_normal_nx.png");
	list.push_back("cm_normal_py.png");
	list.push_back("cm_normal_ny.png");
	list.push_back("cm_normal_pz.png");
	list.push_back("cm_normal_nz.png");
	TextureMgr::Instance()->LoadCM("sky", list);
	TextureMgr::Instance()->Load("soccer", "soccerball.jpg", 4);
	TextureMgr::Instance()->Load("white", "white.png", 4);
	TextureMgr::Instance()->Load("wood", "woodbox.jpg", 4);
	TextureMgr::Instance()->Load("sample", "sample2.jpg", 4);
	TextureMgr::Instance()->Load("rock", "rock.png", 4);
	TextureMgr::Instance()->Load("test_normal", "test_normal.png",1);
	TextureMgr::Instance()->Load("rock2", "rock2.jpg", 10);
	TextureMgr::Instance()->Load("rock2_normal", "rock2.jpg", 10);
	TextureMgr::Instance()->Load("rock2_dp", "rock2.jpg", 10);

	ID3D11ShaderResourceView* sky;
	UINT sampleCount;
	TextureMgr::Instance()->Get("sky", &sky, &sampleCount);
	//canvas->Add("newAnim", XMFLOAT2(0, 50), 760, 320, 0, sampleSRV, sampleCount, 2);

	Skybox* skybox = new Skybox(sky);
	AddObj(skybox);


	UINT texCount;
	ID3D11ShaderResourceView* woodSRV;
	ID3D11ShaderResourceView* soccerSRV;
	ID3D11ShaderResourceView* imageSRV3;
	ID3D11ShaderResourceView* rockSRV;
	ID3D11ShaderResourceView* defaultNormal;
	ID3D11ShaderResourceView* rockNormal;
	ID3D11ShaderResourceView* pbrSRV;
	ID3D11ShaderResourceView* pbrNormal;
	ID3D11ShaderResourceView* pbrDP;
	TextureMgr::Instance()->Get("wood", &woodSRV, &texCount);
	TextureMgr::Instance()->Get("soccer", &soccerSRV, &texCount);
	TextureMgr::Instance()->Get("sample", &imageSRV3, &texCount);
	TextureMgr::Instance()->Get("test_normal", &defaultNormal, &texCount);
	TextureMgr::Instance()->Get("white", &rockSRV, &texCount);
	TextureMgr::Instance()->Get("rock2", &pbrSRV);
	TextureMgr::Instance()->Get("rock2_normal", &pbrNormal);
	TextureMgr::Instance()->Get("rock2_dp", &pbrDP);
	Object* quad = new Object(new Quad(), XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT3(0.1f, 0.1f, 0.1f), XMFLOAT3(0.5f, 0.5f, 0.5f), 4, XMFLOAT3(0, 0, 0), pbrSRV, pbrNormal, sky, 2);
	quad->transform->SetScale(20, 20, 1);
	quad->transform->SetRot(UP, RIGHT);
	quad->transform->SetTranslation(0, 0,0);

	/*Object* sphere = new Object(new Sphere(3), XMFLOAT3(1, 1, 1), XMFLOAT3(0.1f, 0.1f, 0.1f), XMFLOAT3(1, 1, 1), 4, XMFLOAT3(0, 0, 0), rockSRV, rockNormal, sky, 2);
	Object* cylinder = new Object(new Cylinder(20), XMFLOAT3(1, 1, 1), XMFLOAT3(0.1f, 0.1f, 0.1f), XMFLOAT3(1, 1, 1), 4, XMFLOAT3(0, 0, 0), rockSRV, rockNormal, sky, 2);
	sphere->transform->SetScale(20, 20, 20);
	cylinder->transform->SetScale(20, 20, 20);*/


	Shape* dpShape = new Quad();
	dpShape->SetPrimitiveType(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
	DisplacementObject* dpObj = new DisplacementObject(
		dpShape, pbrSRV, pbrNormal, pbrDP, 1.0f,
		XMFLOAT3(1,1,1), XMFLOAT3(1, 1, 1), XMFLOAT3(1, 1, 1),4);
	dpObj->transform->SetScale(20, 20, 1);
	dpObj->transform->SetRot(UP,RIGHT);

	//AddObj(sphere);
	//AddObj(cube);
	//AddObj(cylinder);
	//AddObj(dpObj);
	AddObj(quad);
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
	camera->transform->SetRot(FORWARD * rotMat, UP * rotMat);
}
void TestScene::Logic_Update()
{
	Camera* cam = CameraMgr::Instance()->Main();

	timer->Update();

	CameraMove(cam, timer->SPF());

	Debugging::Instance()->Draw("Main camera = ", cam->transform->GetPos(), 10, 40);

	float elaped = timer->Elapsed();
	if (pLight)
	{
		XMFLOAT3 pt = XMFLOAT3(
			cos(elaped * 0.1f) * 10,
			4,
			sin(elaped * 0.1f) * 10);
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

	/*XMMATRIX rot = XMMatrixRotationX(elaped * 0.2f) * XMMatrixRotationY(elaped*0.15f);
	sphere->transform->SetRot(FORWARD * rot, UP * rot);
	cylinder->transform->SetRot(FORWARD * rot, UP * rot);*/
}

void TestScene::Render_Update(const Camera* camera)
{
	Scene::Render_Update(camera);

	canvas->Update(timer->SPF());
}

void TestScene::Render()const
{
	Scene::Render();

	canvas->Render();
}
