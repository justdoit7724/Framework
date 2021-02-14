#include "stdafx.h"
#include "WndMain.h"

#include "WndDXDisplay.h"

LRESULT WndDefaultProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

WndMain::WndMain(HINSTANCE hInstance, int x, int y, int width, int height)
	:Window(hInstance, L"main"),
	m_DXDisplay(nullptr)
{
	m_hWnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		m_wstrName.c_str(),
		m_wstrName.c_str(),
		WS_POPUP,
		x, y,
		width, height,
		nullptr,
		NULL,
		hInstance,
		nullptr);
	SetWindowLongPtr(m_hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	int mSize = (width > height) ? height : width;
	mSize -= 100;
	m_DXDisplay = new WndDXDisplay(m_hInstance, m_hWnd, width/2- mSize/2, height/2- mSize / 2, mSize, mSize,1);
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
