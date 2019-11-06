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
#include "SSAO.h"
#include "nanosuit.h"

TestScene::TestScene(IGraphic* graphic)
	:Scene("Test"),
	graphic(graphic)
{
	timer = new Timer();
	canvas = new UICanvas(SCREEN_WIDTH, SCREEN_HEIGHT);

	dLight = new DirectionalLight(
		XMFLOAT3(0.5f, 0.5f, 0.5f),
		XMFLOAT3(0.7f, 0.7f, 0.7f),
		XMFLOAT3(0.8f, 0.8f, 0.8f),
		XMFLOAT3(0.707f, -0.707f, 0));
	/*pLight = new PointLight(
		XMFLOAT3(0.5f, 0.5f, 0.5f),
		XMFLOAT3(0.7f, 0.7f, 0.7f),
		XMFLOAT3(0.7f, 0.7f, 0.7f),
		200, XMFLOAT3(0.01f, 0.001f, 0.0005f), XMFLOAT3(0,0,0)
	);*/
	/*pLight2 = new PointLight(
		XMFLOAT3(0.15f, 0.15f, 0.15f),
		XMFLOAT3(0.6f, 0.6f, 0.6f),
		XMFLOAT3(0.7f, 0.7f, 0.7f),
		200, XMFLOAT3(0.01f, 0.001f, 0.0005f), XMFLOAT3(0, 0, 0)
	);*/

	ssao = new SSAO();
	TextureMgr::Instance()->Load("rock" ,"Data\\Texture\\rock.jpg");
	TextureMgr::Instance()->Load("rock_normal", "Data\\Texture\\rock_normal.jpg");
	TextureMgr::Instance()->Load("rock_rough", "Data\\Texture\\rock_rough.jpg");
	TextureMgr::Instance()->Load("simple", "Data\\Texture\\heightmap2.jpg");
	TextureMgr::Instance()->Load("grass", "Data\\Texture\\grass.jpg");
	TextureMgr::Instance()->Load("normal", "Data\\Texture\\default_normal.png");
	std::vector<std::string> cms;
	cms.push_back("Data\\Texture\\cm_normal_px.png");
	cms.push_back("Data\\Texture\\cm_normal_nx.png");
	cms.push_back("Data\\Texture\\cm_normal_py.png");
	cms.push_back("Data\\Texture\\cm_normal_ny.png");
	cms.push_back("Data\\Texture\\cm_normal_pz.png");
	cms.push_back("Data\\Texture\\cm_normal_nz.png");
	TextureMgr::Instance()->LoadCM("skybox", cms);
	ID3D11ShaderResourceView* pbrSRV= TextureMgr::Instance()->Get("rock");
	ID3D11ShaderResourceView* pbrNormal= TextureMgr::Instance()->Get("rock_normal");
	ID3D11ShaderResourceView* pbrRough= TextureMgr::Instance()->Get("rock_rough");
	ID3D11ShaderResourceView* simpleSRV= TextureMgr::Instance()->Get("simple");
	ID3D11ShaderResourceView* grassSRV= TextureMgr::Instance()->Get("grass");
	ID3D11ShaderResourceView* defaultNormal = TextureMgr::Instance()->Get("normal");
	ID3D11ShaderResourceView* cmSRV = TextureMgr::Instance()->Get("skybox");
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////

	DX_DContext->PSSetShaderResources(5, 1, &simpleSRV);
	DX_DContext->PSSetShaderResources(4, 1, &pbrRough);
	DX_DContext->PSSetShaderResources(0, 1, &cmSRV);
	//canvas->Add("TEST", XMFLOAT2(0, 0), 300, 300, 0, grassSRV);

	Object* dcm = new DynamicCubeMap(this, new Sphere(3));
	dcm->transform->SetScale(20, 20, 20);
	dcm->transform->SetTranslation(-40, 40, 40);
	//AddObj(dcm);

	const int N = 3;
	for (int y = 0; y < N; ++y) {
		for (int x = 0; x < N; ++x) {
			float interval = 15;
			XMFLOAT3 scale = XMFLOAT3(5, 5, 5);
			XMFLOAT3 offset = XMFLOAT3(scale.x,0,scale.z)* (N / 2);
			Object* cube = new Object(new Sphere(4), pbrSRV, defaultNormal);
			cube->transform->SetScale(scale);
			cube->transform->SetTranslation(XMFLOAT3(x * interval,10.0f + y * interval, 0)-offset);
			AddObj(cube);
			//Debugging::Instance()->Visualize(cube);
		}
	}

	nanosuit* mesh = new nanosuit();
	mesh->SetScale(XMFLOAT3(3, 3, 3));
	for (auto obj : mesh->objs)
	{
		//AddObj(obj);
	}

	Debugging::Instance()->EnableGrid(10);
}

TestScene::~TestScene()
{
	delete canvas;
	delete dLight;
}


void TestScene::Update(float elapsed, float spf)
{
	if(pLight)
		pLight->Update();
	timer->Update();

	float elaped = timer->Elapsed();
	if (dLight)
	{
		XMFLOAT3 pt = MultiplyDir(XMFLOAT3(
			150,
			150,
			0), XMMatrixRotationY(elaped * 0.5f));
		XMFLOAT3 dir = Normalize(XMFLOAT3(-pt.x, -pt.y, -pt.z));
		dLight->SetDir(dir);
	}
	if (pLight)
	{
		XMFLOAT3 pt = XMFLOAT3(
			cos(elaped*0.5) * 50,
			20.0f,
			sin(elaped * 0.5) * 50);
		pLight->SetPos(pt);
		Debugging::Instance()->Mark(pt, 1.5f, Colors::WhiteSmoke);
	}
	if (pLight2)
	{
		XMFLOAT3 pt = XMFLOAT3(
			cos(elaped * 0.15f) * 15,
			cos(elaped*0.1f) * 2,
			sin(elaped * 0.15f) * 25);
		pLight2->SetPos(pt);
		//Debugging::Instance()->Mark(pt, 1.5f, Colors::Red);
		//Debugging::Instance()->Draw("Pos = ", pt, 10, 10);
	}


	const Camera* mainCam = CameraMgr::Instance()->Main();
	FrustumCulling(mainCam);

	Scene::Update(elaped, spf);
	

	/*Geometrics::Ray pickRay;
	mainCam->Pick(&pickRay.o, &pickRay.d);
	for (int i = 0; i < drawObjs.size(); ++i)
	{

	}*/

	//ssao->Update(mainCam, drawObjs);

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