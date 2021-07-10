#include "stdafx.h"
#include "WndMain.h"

#include "WndPaneAsset.h"
#include "CtrlButton.h"


enum {
	ID_CTRL_KEY_SCENE,
	ID_CTRL_KEY_ASSET
};


WndMain::WndMain(HINSTANCE hInstance)
	:WndBkg(hInstance, "WndMain", WS_MAXIMIZE | WS_OVERLAPPED, nullptr, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN))
{
	m_sceneBtn = new CtrlButton(hInstance, m_hWnd, "scene", 20, 20, 200, 75, ID_CTRL_KEY_SCENE);
	m_sceneBtn->SetClickFunc(std::bind(&WndMain::OpenWndScene, this));
	m_assetBtn = new CtrlButton(hInstance, m_hWnd, "asset", 20, 120, 200, 75, ID_CTRL_KEY_ASSET);
	m_assetBtn->SetClickFunc(std::bind(&WndMain::OpenWndAsset, this));
}

WndMain::~WndMain()
{
	SAFEDELETE(m_sceneBtn);
	SAFEDELETE(m_assetBtn);
	DestroyWindow(m_hWnd);
}

void WndMain::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	WndBkg::WndProc(hwnd, msg, wparam, lparam);

	switch (msg)
	{
	case WM_KEYDOWN:
	{
		switch (wparam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
	}
	break;
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	}
}

void WndMain::OpenWndScene()
{
}
void WndMain::OpenWndAsset()
{
	WndBkg* newPane = new WndPaneAsset(m_hInstance, m_hWnd);
	newPane->ShowWindow(TRUE);
}
