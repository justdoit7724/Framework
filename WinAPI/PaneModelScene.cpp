#include "stdafx.h"
#include "PaneModelScene.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Object.h"
#include "Light.h"

using namespace DX;

PaneModelScene::PaneModelScene(DX::Graphic* graphic)
	:Scene(graphic, "Model")
{
	m_light = new Object;
	m_light->AddComponent(CMPNT_KIND::Light);
	Light* dirLight =  (Light*)m_light->GetComponent(CMPNT_KIND::Light);


	/*
	m_dLight = new DX::DirectionalLight(
		graphic->Device(),
		0,
		XMFLOAT3(0.4, 0.4, 0.4),
		XMFLOAT3(0.8, 0.8, 0.8),
		XMFLOAT3(0.7, 0.7, 0.7),
		1.7f,
		DX::Normalize(XMFLOAT3(1, -0.5f, 1))
	);
	m_dLight->Enable(true);

	m_camera = new DX::Camera("cam", DX::FRAME_KIND_PERSPECTIVE, NULL, NULL, 0.001f, 500, XM_PIDIV2, 1, false);
	m_camera->transform->SetTranslation(0, 0, 0);
	m_camera->transform->SetRot(DIR_FORWARD);
	m_camera->Update();

	m_cbEye = new DX::Buffer(graphic->Device(), sizeof(XMFLOAT4));

	CreateGrid(0, 0, XMFLOAT3(100, 100, 100));
	*/
}

PaneModelScene::~PaneModelScene()
{
	/*for (int i = 0; i < m_vMain.size(); ++i)
	{
		delete m_vMain[i];
	}
	for (int i = 0; i < m_vSkeleton.size(); ++i)
	{
		delete m_vSkeleton[i];
	}
	m_vMain.clear();
	m_vSkeleton.clear();

	m_dxRedSRV->Release();
	m_dxBlueSRV->Release();
	m_dxGreenSRV->Release();
	m_dxWhiteSRV->Release();
	m_dxNormSRV->Release();
	SAFEDELETE(m_dLight);
	SAFEDELETE(m_camera);
	SAFEDELETE(m_cbEye);
	m_dxSamp->Release();*/
	
}


void PaneModelScene::Update()
{
	Scene::Update();
}

void PaneModelScene::Open(std::string path)
{
	//release
	/*for (int i = 0; i < m_vMain.size(); ++i)
	{
		delete m_vMain[i];
	}
	for (int i = 0; i < m_vSkeleton.size(); ++i)
	{
		delete m_vSkeleton[i];
	}
	m_vMain.clear();
	m_vSkeleton.clear();


	DX::ModelLoader::Load(m_dxGraphic->Device(),m_dxGraphic->DContext(),
		"C:\\Users\\Jun\\Desktop\\Cube.x",
		&m_mesh);

	Geometrics::Bound outBound;
	m_mesh->GetLBound(&outBound);
	XMFLOAT3 modelSize = outBound.maxPos - outBound.minPos;

	CreateGrid(50, 50, modelSize*2);

	for (int i = 0; i < m_mesh->m_nSubMesh; ++i)
	{
		DX::SubMesh* subMesh;
		m_mesh->PassSubMesh(i, &subMesh);
		std::shared_ptr<DX::Mesh> singleMesh = std::make_shared<DX::Mesh>(1);
		singleMesh->SetSubMesh(0, subMesh);
		DX::Object* dxObj;
		dxObj = new DX::Object(
			m_dxGraphic->Device(), m_dxGraphic->DContext(),
			"subMesh Obj", singleMesh, nullptr,
			m_dxRedSRV, m_dxNormSRV);
		dxObj->transform->SetTranslation(0, 0, 0);
		dxObj->transform->SetScale(1, 1, 1);
		m_vMain.push_back(dxObj);
	}*/
}

void PaneModelScene::CreateGrid(int nX, int nY, XMFLOAT3 size)
{
	/*
	m_gridMesh = std::make_shared<DX::LineMesh>();

	// x,y,z 기준 
	m_gridMesh->Add(XMFLOAT3(0, 0, 0), XMFLOAT3(size.x/2, 0, 0));
	m_gridMesh->Generate(m_dxGraphic->Device(), 0);
	m_gridMesh->Clear();
	m_gridMesh->Add(XMFLOAT3(0, 0, 0), XMFLOAT3(0, size.y/2, 0));
	m_gridMesh->Generate(m_dxGraphic->Device(), 1);
	m_gridMesh->Clear();
	m_gridMesh->Add(XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, size.z/2));
	m_gridMesh->Generate(m_dxGraphic->Device(), 2);
	m_gridMesh->Clear();

	// 나머지
	for (int x = -nX / 2; x < nX / 2; ++x)
	{
		if (x == 0)
			continue;
		XMFLOAT3 p1(x, -size.y/2, 0);
		XMFLOAT3 p2(x, size.y/2, 0);
		m_gridMesh->Add(p1, p2);
	}
	for (int y = -nY/2; y < nY/2; ++y)
	{
		if (y == 0)
			continue;
		XMFLOAT3 p1(-size.x/2, y, 0);
		XMFLOAT3 p2(size.x/2, y, 0);
		m_gridMesh->Add(p1, p2);
	}
	m_gridMesh->Generate(m_dxGraphic->Device(),4);*/
}
