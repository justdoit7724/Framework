#include "stdafx.h"
#include "WndDisplay.h"
#include "Scene.h"
#include "Timer.h"
#include "Keyboard.h"
#include "Mouse.h"

WndDisplay::WndDisplay(HINSTANCE hInstance, HWND parent, int x, int y, int width, int height, int msaa)
	:Window(hInstance, "Display"), 
	m_iMSAA(msaa)
{
	assert(msaa == 1 || msaa == 2 || msaa == 4 || msaa == 8 || msaa == 16);
	
	RegisterWnd();
	CreateWnd(parent, NULL, WS_CHILD, x, y, width, height);


	m_dxGraphic = new DX::Graphic;
	int ret = m_dxGraphic->Initialize(m_hWnd, m_iMSAA);


}

WndDisplay::~WndDisplay()
{
	if (m_dxGraphic)
		delete m_dxGraphic;
	DestroyWindow(m_hWnd);
}

void WndDisplay::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_COMMAND:

		switch (LOWORD(wparam))
		{
			case ID_COMMAND_REALTIME_UPDATE:
			{
				for (auto scene : m_scenes)
				{
					scene->Update();
					scene->Render();
				}
				m_dxGraphic->Present();
			}
			break;
			
		}
		break;
	case WM_LBUTTONDOWN:
		SetFocus(m_hWnd);
		SGL_Mouse.Click(TRUE);
		break;
	case WM_LBUTTONUP:
		SGL_Mouse.Release(TRUE);
		break;
	case WM_RBUTTONDOWN:
	{
		SetFocus(m_hWnd); 
		SGL_Mouse.Click(FALSE);
	}
	break;
	case WM_RBUTTONUP:
	{
		SGL_Mouse.Release(FALSE);
	}
	break;
	case WM_KEYDOWN:
	{
		SGL_Keyboard.Press(wparam);
	}
	break;
	case WM_KEYUP:
	{
		SGL_Keyboard.Release(wparam);
	}
	break;
	case WM_MOUSEMOVE:
		SGL_Mouse.Move(lparam);
		break;
	case WM_MOUSEWHEEL:
		SGL_Mouse.Wheel(lparam);
	}
}

BOOL WndDisplay::AddScene(SCENE_KIND kind)
{
	m_scenes.push_back(Scene::CreateScene(kind, m_dxGraphic));

	return TRUE;
}
