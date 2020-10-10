#include "stdafx.h"
#include "WndDXDisplay.h"
#include "StartScene.h"
#include "Timer.h"

WndDXDisplay::WndDXDisplay(HINSTANCE hInstance, HWND parent, int x, int y, int width, int height)
	:Window(hInstance, x,y,width,height, L"DX display")
{
	m_hWnd = CreateWindowEx(
		0,
		m_wstrName.c_str(),
		m_wstrName.c_str(),
		(DWORD)WindowType::Popup,
		x, y,
		width, height,
		parent,
		NULL,
		hInstance,
		this);

	m_dxGraphic = new DX::Graphic(m_hWnd, GetWidth(), GetHeight());

	m_scene = new StartScene(m_dxGraphic->Device(), m_dxGraphic->DContext(), L"start");
}

WndDXDisplay::~WndDXDisplay()
{
}

void WndDXDisplay::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (m_scene)
		m_scene->WndProc(msg, wparam, lparam);

	switch (msg)
	{
	case WM_CREATE:

		break;
	case WM_COMMAND:

		switch (LOWORD(wparam))
		{
		case ID_COMMAND_UPDATE:
		{
			m_dxGraphic->Present();
		}
			break;
		case ID_RADIO_MAIN_DIRECTIONAL_LIGHT:
			break;
		case ID_RADIO_MAIN_POINT_LIGHT:
			break;
		case ID_RADIO_MAIN_SPOT_LIGHT:
			break;
		}

		break;
	}

}
