#include "stdafx.h"
#include "WndMain.h"

#include "WndDXDisplay.h"

LRESULT WndDefaultProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

WndMain::WndMain(HINSTANCE hInstance, int x, int y, int width, int height)
	:Window(hInstance, L"main"),
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


	m_DXDisplay = new WndDXDisplay(m_hInstance, m_hWnd, 30, 30, 600, 600,1);
	m_DXDisplay->ShowWindow();
}

WndMain::~WndMain()
{
	delete m_DXDisplay;
	DestroyWindow(m_hWnd);

}

void WndMain::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_COMMAND:

		switch (LOWORD(wparam))
		{
		case ID_COMMAND_REALTIME_UPDATE:
			if (IsWindowVisible(m_DXDisplay->HWnd()))
				SendMessage(m_DXDisplay->HWnd(), msg, wparam, lparam);
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
