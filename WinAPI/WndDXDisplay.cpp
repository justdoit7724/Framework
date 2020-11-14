#include "stdafx.h"
#include "WndDXDisplay.h"
#include "VisualAAScene.h"
#include "PlayScene.h"
#include "Timer.h"

WndDXDisplay::WndDXDisplay(HINSTANCE hInstance, HWND parent, int x, int y, int width, int height)
	:Window(hInstance, L"DX display"), m_curScene(nullptr)
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
		nullptr);
	SetWindowLongPtr(m_hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	m_dxGraphic = new DX::Graphic(m_hWnd);

	m_visualAAScene = new VisualAAScene(m_dxGraphic->Device(), m_dxGraphic->DContext(), L"Visual");
	m_playScene = new PlayScene(m_dxGraphic->Device(), m_dxGraphic->DContext(), L"Play");

}

WndDXDisplay::~WndDXDisplay()
{
}

void WndDXDisplay::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_COMMAND:

		switch (LOWORD(wparam))
		{
		case ID_COMMAND_REALTIME_UPDATE:
		{
			Timer* timer = (Timer*)lparam;

			if(m_curScene)
				m_curScene->Update(timer->Elapsed(), timer->SPF());

			m_dxGraphic->Present();
		}
			break;
		case ID_CONTROL_MAIN_VISUAL:
			m_curScene = m_visualAAScene;
			m_curScene->WM_Resize(GetWidth(), GetHeight());
			break;
		case ID_CONTROL_MAIN_PLAY:
			m_curScene = m_playScene;
			m_curScene->WM_Resize(GetWidth(), GetHeight());
			break;
		}

		break;

	case WM_MOUSEMOVE:
		if (m_curScene)
		m_curScene->WM_MouseMove(lparam);
		break;
	case WM_LBUTTONDOWN:
		SetFocus(m_hWnd);
		break;
	case WM_LBUTTONUP:
		break;
	case WM_RBUTTONDOWN:
		SetFocus(m_hWnd);
		if (m_curScene)
		m_curScene->WM_RButtonDown();
		break;
	case WM_RBUTTONUP:
		if (m_curScene)
		m_curScene->WM_RButtonUp();
		break;
	case WM_KEYDOWN:
		if (m_curScene)
		m_curScene->WM_KeyDown(wparam);
		break;
	case WM_KEYUP:
		if (m_curScene)
		m_curScene->WM_KeyUp(wparam);
		break;
	break;
	}

}
