#include "stdafx.h"
#include "WndDX.h"
#include "Window.h"
#include "SceneMgr.h"
#include "Timer.h"

#include "StartScene.h"

WndDX::WndDX(HWND hwnd) {

	m_timer = new Timer();

	RECT rect;
	GetClientRect(hwnd, &rect);

	m_pEnv = new DX::Graphic(hwnd, rect.right - rect.left, rect.bottom - rect.top);
	m_device = m_pEnv->Device();
	m_dContext = m_pEnv->DContext();

	startScene = new StartScene(m_device, m_dContext, L"Start");

	SceneMgr::Instance()->Add(L"startScene", startScene);
}

WndDX::~WndDX()
{
	delete m_pEnv;
	delete m_timer;
}

void WndDX::Update()
{
	m_timer->Update();

	SceneMgr::Instance()->Process(m_timer->Elapsed(), m_timer->SPF());

	m_pEnv->Present();
}

void WndDX::WndProc(unsigned int msg, unsigned long long wparam, long long lparam)
{
	switch (msg)
	{
	case WM_KEYUP:
		startScene->ReleaseKey(wparam);
		break;
	case WM_KEYDOWN:
		startScene->PressKey(wparam);
		break;
	case WM_LBUTTONDOWN:
		startScene->Mouse_LBtnDown();
		break;
	case WM_LBUTTONUP:
		startScene->Mouse_LBtnUp();
		break;
	case WM_RBUTTONDOWN:
		startScene->Mouse_RBtnDown();
		break;
	case WM_RBUTTONUP:
		startScene->Mouse_RBtnUp();
		break;
	case WM_MOUSEMOVE:
		startScene->Mouse_UpdatePt(lparam);
		break;
	case WM_MOUSEWHEEL:
		startScene->Mouse_Wheel(wparam);
		break;
	default:
		break;
	}
}
