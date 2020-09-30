
#include "pch.h"

#include "TestScene.h"
#include "Camera.h"
#include "Debugging.h"
#include "TextureMgr.h"
#include "UI.h"

using namespace DX;

TestScene::TestScene()
{
	m_pCanvas = new UICanvas();

	//TextureMgr::Instance()->Load("sample", "DXFramework\\Data\\Texture\\sample.jpg");

	//m_pTestUI = new UI(XMFLOAT2(0, 0), XMFLOAT2(160, 100), 0, TextureMgr::Instance()->Get("sample"));
	

	m_pCam = new Camera("cam", FRAME_KIND_PERSPECTIVE, NULL, NULL, 1.0f, 1000.0f, XM_PIDIV2, 1, false);
	Debugging::Instance()->SetCamera(m_pCam);
	Debugging::Instance()->EnableGrid(10);
}

TestScene::~TestScene()
{
	delete m_pCam;
}

void TestScene::Update(float elapsed, float spf)
{
	m_pCam->Move(spf);
}

