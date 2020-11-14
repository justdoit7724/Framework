#include "stdafx.h"
#include "WndMain.h"

#include "WndDXDisplay.h"



LRESULT WndDefaultProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);



WndMain::WndMain(HINSTANCE hInstance, int x, int y, int width, int height)
	:Window(hInstance, L"main"),
	m_hRadioVisualVersion(nullptr), m_hRadioPlayVersion(nullptr),
	m_hComboResolution(nullptr),
	m_DXDisplay(nullptr)
{
	m_hWnd = CreateWindowEx(
		0,
		m_wstrName.c_str(),
		m_wstrName.c_str(),
		(DWORD)WindowType::Frame,
		x, y,
		width, height,
		nullptr,
		NULL,
		hInstance,
		nullptr);
	SetWindowLongPtr(m_hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	m_hRadioVisualVersion = CreateWindow(
		L"button",
		L"Test visualization",
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP,
		700, 20, 100, 30,
		m_hWnd,
		(HMENU)ID_CONTROL_MAIN_VISUAL,
		m_hInstance,
		NULL
	);
	m_hRadioPlayVersion = CreateWindow(
		L"button",
		L"Practical play",
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
		700, 50, 100, 30,
		m_hWnd,
		(HMENU)ID_CONTROL_MAIN_PLAY,
		m_hInstance,
		NULL
	);

	m_DXDisplay = new WndDXDisplay(m_hInstance, m_hWnd, 30, 30, 600, 600);
	m_DXDisplay->ShowWindow();

	SetRadioSceneVersion(ID_CONTROL_MAIN_VISUAL);
}

void WndMain::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_COMMAND:

		switch (LOWORD(wparam))
		{
		case ID_COMMAND_REALTIME_UPDATE:
			SendMessage(m_DXDisplay->HWnd(), msg, wparam, lparam);
			break;
		case ID_CONTROL_MAIN_VISUAL:
		case ID_CONTROL_MAIN_PLAY:
			SetRadioSceneVersion(wparam);
			break;
		case ID_CONTROL_RESOLUTION_1:
		case ID_CONTROL_RESOLUTION_2:
		case ID_CONTROL_RESOLUTION_4:
		case ID_CONTROL_RESOLUTION_8:
		case ID_CONTROL_RESOLUTION_16:
			SetComboResolution(wparam);
			break;
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

void WndMain::SetComboResolution(int setID)
{
}

void WndMain::SetRadioSceneVersion(int setID)
{
	switch (setID)
	{
	case ID_CONTROL_MAIN_VISUAL:
		SendMessage(m_hRadioVisualVersion, BM_SETCHECK, BST_CHECKED, NULL);
		SendMessage(m_hRadioPlayVersion, BM_SETCHECK, BST_UNCHECKED, NULL);
		break;
	case ID_CONTROL_MAIN_PLAY:
		SendMessage(m_hRadioVisualVersion, BM_SETCHECK, BST_UNCHECKED, NULL);
		SendMessage(m_hRadioPlayVersion, BM_SETCHECK, BST_CHECKED, NULL);
		break;
	}

	SendMessage(m_DXDisplay->HWnd(), WM_COMMAND, setID, NULL);
}
