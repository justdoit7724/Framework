#include "stdafx.h"
#include "WndMain.h"

#include "WndDisplay.h"
#include "WndPaneModel.h"

LRESULT WndDefaultProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

WndMain::WndMain(HINSTANCE hInstance)
	:Window(hInstance, L"main", WindowType::Frame, nullptr, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)),
	m_DXDisplay(nullptr)
{
	m_hPaneScene = CreateWindow(
		L"button",
		L"Scene",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		20, 20, 200, 75,
		m_hWnd,
		(HMENU)ID_BTN_PANE_SCENE,
		hInstance,
		NULL);
	m_hPaneModel= CreateWindow(
		L"button",
		L"Model",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		20, 120, 200, 75,
		m_hWnd,
		(HMENU)ID_BTN_PANE_MODEL,
		hInstance,
		NULL);
	m_hPaneAnimation = CreateWindow(
		L"button",
		L"Animation",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		20, 220, 200, 75,
		m_hWnd,
		(HMENU)ID_BTN_PANE_ANIMATION,
		hInstance,
		NULL);
}

WndMain::~WndMain()
{
	DestroyWindow(m_hPaneScene);
	DestroyWindow(m_hPaneModel);
	DestroyWindow(m_hPaneScene);
	DestroyWindow(m_hWnd);
}

void WndMain::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_COMMAND:

		switch (LOWORD(wparam))
		{
		case ID_BTN_PANE_SCENE:
			break;
		case ID_BTN_PANE_MODEL:
		{
			Window* newPane = new WndPaneModel(m_hInstance, m_hWnd);
			newPane->ShowWindow(TRUE);
		}
			break;
		case ID_BTN_PANE_ANIMATION:
			break;
		}
		break;

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
