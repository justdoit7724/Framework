#include "stdafx.h"
#include "WndDXDisplay.h"
#include "PlayScene.h"
#include "Timer.h"

WndDXDisplay::WndDXDisplay(HINSTANCE hInstance, HWND parent, int x, int y, int width, int height, int msaa)
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

	m_dxGraphic = new DX::Graphic(m_hWnd, m_iMSAA);
	m_scene = new PlayScene(m_dxGraphic, L"main");
}

WndDXDisplay::~WndDXDisplay()
{
	if(m_scene)
		delete m_scene;
	if (m_dxGraphic)
		delete m_dxGraphic;
	DestroyWindow(m_hWnd);
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

				m_scene->Update(timer->Elapsed(), timer->SPF());
				m_dxGraphic->Present();
			}
			break;
			
			case ID_CONTROL_TRANSPARENCY:

				m_scene->SetTransparency(lparam);
				break;
		}
		break;
	case WM_LBUTTONDOWN:
		SetFocus(m_hWnd);
		m_scene->WM_LButtonDown();
		break;
	case WM_LBUTTONUP:
		m_scene->WM_LButtonUp();
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
		switch (wparam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			return;
		}
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

