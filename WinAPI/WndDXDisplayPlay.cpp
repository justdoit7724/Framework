#include "stdafx.h"
#include "WndDXDisplayPlay.h"
#include "PlayScene.h"
#include "Timer.h"

WndDXDisplayPlay::WndDXDisplayPlay(HINSTANCE hInstance, HWND parent, int x, int y, int width, int height, int msaa)
	:Window(hInstance, L"DX DisplayRight"), m_iMSAA(msaa)
{
	assert(msaa == 1 || msaa == 2 || msaa == 4 || msaa == 8 || msaa == 16);

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

}

WndDXDisplayPlay::~WndDXDisplayPlay()
{
	if(m_scene)
		delete m_scene;
}

void WndDXDisplayPlay::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_SHOWWINDOW:

		SAFEDELETE(g_dxGraphic);
		SAFEDELETE(m_scene);
		g_dxGraphic = new DX::Graphic(m_hWnd, m_iMSAA);
		m_scene = new PlayScene(g_dxGraphic->Device(), g_dxGraphic->DContext(), L"main");
		break;
	case WM_COMMAND:

		switch (LOWORD(wparam))
		{
			case ID_COMMAND_REALTIME_UPDATE:
			{
				Timer* timer = (Timer*)lparam;

				m_scene->Update(timer->Elapsed(), timer->SPF());
				g_dxGraphic->Present();
			}
			break;
			case ID_CONTROL_RESOLUTION:
			{
				m_iMSAA = HIWORD(wparam);
				SAFEDELETE(g_dxGraphic);
				SAFEDELETE(m_scene);
				g_dxGraphic = new DX::Graphic(m_hWnd, m_iMSAA);
				m_scene = new PlayScene(g_dxGraphic->Device(), g_dxGraphic->DContext(), L"main");
			}
			break;
			
		}
		break;
	case WM_LBUTTONDOWN:
		SetFocus(m_hWnd);
		break;
	case WM_RBUTTONDOWN:
	{
		SetFocus(m_hWnd);
		m_scene->WM_RButtonDown();
	}
	break;
	case WM_RBUTTONUP:
	{
		m_scene->WM_RButtonUp();
	}
	break;
	case WM_KEYDOWN:
	{
		m_scene->WM_KeyDown(wparam);
	}
	break;
	case WM_KEYUP:
	{
		m_scene->WM_KeyUp(wparam);
	}
	break;
	case WM_MOUSEMOVE:
		m_scene->WM_MouseMove(lparam);
		break;
	}
}
