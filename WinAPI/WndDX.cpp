#include "stdafx.h"
#include "WndDX.h"
#include "Window.h"
#include "SceneMgr.h"

#include "StartScene.h"

WndDX::WndDX(HWND hwnd) {

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
}

void WndDX::Update()
{
	startScene->Update(0, 0);

	m_pEnv->Present();
}

void WndDX::ReleaseKey(WPARAM wparam)
{
}

void WndDX::PressKey(WPARAM wparam)
{
}

void WndDX::Mouse_LBtnDown()
{
}

void WndDX::Mouse_LBtnUp()
{
}

void WndDX::Mouse_RBtnDown()
{
}

void WndDX::Mouse_RBtnUp()
{
}

void WndDX::Mouse_UpdatePt(LPARAM lparam)
{
}

void WndDX::Mouse_Wheel(WPARAM wparam)
{
}

void WndDX::WndProc(unsigned int msg, unsigned long long wparam, long long lparam)
{
	switch (msg)
	{
	case WM_KEYUP:
		ReleaseKey(wparam);
		break;
	case WM_KEYDOWN:
		PressKey(wparam);
		break;
	case WM_LBUTTONDOWN:
		Mouse_LBtnDown();
		break;
	case WM_LBUTTONUP:
		Mouse_LBtnUp();
		break;
	case WM_RBUTTONDOWN:
		Mouse_RBtnDown();
		break;
	case WM_RBUTTONUP:
		Mouse_RBtnUp();
		break;
	case WM_MOUSEMOVE:
		Mouse_UpdatePt(lparam);
		break;
	case WM_MOUSEWHEEL:
		Mouse_Wheel(wparam);
		break;
	default:
		break;
	}
}
