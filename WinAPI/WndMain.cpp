#include "stdafx.h"
#include "WndMain.h"

#include "WndDXDisplay.h"



LRESULT WndDefaultProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);



WndMain::WndMain(HINSTANCE hInstance, int x, int y, int width, int height)
	:Window(hInstance, x, y, width, height, L"main"),
	m_hCheckDLight(nullptr), m_hCheckPLight(nullptr), m_hCheckSLight(nullptr),
	m_DXDisplay(nullptr)
{
	m_hWnd = CreateWindowEx(
		0,
		m_wstrName.c_str(),
		m_wstrName.c_str(),
		(DWORD)WindowType::Frame,
		x, y,
		width, height,
		NULL,
		NULL,
		hInstance,
		this);

	/*RECT clientRect;
	GetClientRect(m_hWnd, &clientRect);
	POINT offset;
	offset.x = width - clientRect.right;
	offset.y = height - clientRect.bottom;
	MoveWindow(m_hWnd, x, y, width + offset.x, height + offset.y, FALSE);*/

	m_DXDisplay = new WndDXDisplay(m_hInstance, m_hWnd, 50, 50, 600, 600);
	m_DXDisplay->ShowWindow();
	
	m_hCheckDLight = CreateWindow(L"button", L"directional light", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 800, 50, 100, 30, m_hWnd, (HMENU)ID_CHECK_MAIN_DIRECTIONAL_LIGHT, m_hInstance, NULL);
	m_hCheckPLight = CreateWindow(L"button", L"point light", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 800, 80, 100, 30, m_hWnd, (HMENU)ID_CHECK_MAIN_POINT_LIGHT, m_hInstance, NULL);
	m_hCheckSLight = CreateWindow(L"button", L"spot light", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 800, 110, 100, 30, m_hWnd, (HMENU)ID_CHECK_MAIN_SPOT_LIGHT, m_hInstance, NULL);
}

void WndMain::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_NCCREATE:

		break;
	case WM_COMMAND:

		switch (LOWORD(wparam))
		{
		case ID_COMMAND_REALTIME_UPDATE:
			SendMessage(m_DXDisplay->HWnd(), msg, wparam, lparam);
			break;
		case ID_CHECK_MAIN_DIRECTIONAL_LIGHT:
		{
			LRESULT lrCheck = SendMessage(m_hCheckDLight, BM_GETCHECK, NULL, NULL);

			SetCheckBoxLight(ID_CHECK_MAIN_DIRECTIONAL_LIGHT, lrCheck ==BST_CHECKED);

			break;
		}
		case ID_CHECK_MAIN_POINT_LIGHT:
		{
			LRESULT lrCheck = SendMessage(m_hCheckPLight, BM_GETCHECK, NULL, NULL);

			SetCheckBoxLight(ID_CHECK_MAIN_POINT_LIGHT, lrCheck == BST_CHECKED);

			break;
		}
		case ID_CHECK_MAIN_SPOT_LIGHT:
		{
			LRESULT lrCheck = SendMessage(m_hCheckSLight, BM_GETCHECK, NULL, NULL);

			SetCheckBoxLight(ID_CHECK_MAIN_SPOT_LIGHT, lrCheck == BST_CHECKED);

			break;
		}
		}
		break;

	case WM_KEYDOWN:
	{
		if (wparam == VK_ESCAPE)
			PostQuitMessage(0);

		break;
	}
	case WM_CLOSE:

		PostQuitMessage(0);
		break;
	}
}

void WndMain::SetCheckBoxLight(int setID, bool bCheck)
{
	switch (setID)
	{
	case ID_CHECK_MAIN_DIRECTIONAL_LIGHT:
	case ID_CHECK_MAIN_POINT_LIGHT:
	case ID_CHECK_MAIN_SPOT_LIGHT:
		SendMessage(m_DXDisplay->HWnd(), WM_COMMAND, setID, bCheck? 1:0);
		break;
	}
}
