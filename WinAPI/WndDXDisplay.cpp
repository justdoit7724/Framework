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
	switch (msg)
	{
	case WM_CREATE:

		break;
	case WM_COMMAND:

		switch (LOWORD(wparam))
		{
		case ID_COMMAND_REALTIME_UPDATE:
		{
			Timer* timer = (Timer*)lparam;

			m_scene->Update(timer->Elapsed(), timer->SPF());

			m_dxGraphic->Present();
		}
			break;
		case ID_CHECK_MAIN_DIRECTIONAL_LIGHT:
		{
			bool bCheck = lparam;

			m_scene->EnableDLight(bCheck);
		}
			break;
		case ID_CHECK_MAIN_POINT_LIGHT:
		{
			bool bCheck = lparam;

			m_scene->EnablePLight(bCheck);
			break;
		}
		case ID_CHECK_MAIN_SPOT_LIGHT:
		{
			bool bCheck = lparam;

			m_scene->EnableSLight(bCheck);
			break;
		}
		}

		break;

	case WM_MOUSEMOVE:
		m_scene->MouseMove(lparam);
		break;
	case WM_LBUTTONDOWN:
		SetFocus(m_hWnd);
		break;
	case WM_LBUTTONUP:
		break;
	case WM_RBUTTONDOWN:
		SetFocus(m_hWnd);
		m_scene->RButtonDown();
		break;
	case WM_RBUTTONUP:
		m_scene->RButtonUp();
		break;
	case WM_KEYDOWN:
		m_scene->KeyDown(wparam);
		break;
	case WM_KEYUP:
		m_scene->KeyUp(wparam);
		break;
	}

}
