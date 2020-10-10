#include "stdafx.h"
#include "WndMain.h"

#include "WndDXDisplay.h"





WndMain::WndMain(HINSTANCE hInstance, int x, int y, int width, int height)
	:Window(hInstance, x, y, width, height, L"main"),
	m_hRadioDLight(nullptr), m_hRadioPLight(nullptr), m_hRadioSLight(nullptr),
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

	m_hRadioDLight = CreateWindow(L"button", L"directional light", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON | WS_GROUP, 800, 50, 100, 30, m_hWnd, (HMENU)ID_RADIO_MAIN_DIRECTIONAL_LIGHT, m_hInstance, NULL);
	m_hRadioPLight = CreateWindow(L"button", L"point light", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 800, 80, 100, 30, m_hWnd, (HMENU)ID_RADIO_MAIN_POINT_LIGHT, m_hInstance, NULL);
	m_hRadioSLight = CreateWindow(L"button", L"spot light", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 800, 110, 100, 30, m_hWnd, (HMENU)ID_RADIO_MAIN_SPOT_LIGHT, m_hInstance, NULL);
	SetRadioLight(ID_RADIO_MAIN_DIRECTIONAL_LIGHT);

	m_DXDisplay = new WndDXDisplay(m_hInstance, m_hWnd, 50, 50, 600, 600);
	m_DXDisplay->ShowWindow();
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
		case ID_COMMAND_UPDATE:
			break;
		case ID_RADIO_MAIN_DIRECTIONAL_LIGHT:
		case ID_RADIO_MAIN_POINT_LIGHT:
		case ID_RADIO_MAIN_SPOT_LIGHT:
			SetRadioLight(LOWORD(wparam));
			break;
		}

		break;
	case WM_CLOSE:

		PostQuitMessage(0);
		break;
	}

	if (m_DXDisplay)
		m_DXDisplay->WndProc(msg, wparam, lparam);
}

void WndMain::SetRadioLight(int setID)
{
	CheckRadioButton(m_hWnd, ID_RADIO_MAIN_DIRECTIONAL_LIGHT, ID_RADIO_MAIN_SPOT_LIGHT, setID);

	switch (setID)
	{
	case ID_RADIO_MAIN_DIRECTIONAL_LIGHT:
	{
		int a = 0;
	}
		break;
	case ID_RADIO_MAIN_POINT_LIGHT:
	{
		int a = 0;
	}
		break;
	case ID_RADIO_MAIN_SPOT_LIGHT:
	{
		int a = 0;
	}
		break;
	}
}
