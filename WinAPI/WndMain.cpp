#include "stdafx.h"
#include "WndMain.h"

#include "WndDXDisplay.h"

const TCHAR* items[] = 
{ 
	L"Normal Transparency", 
	L"1 Depth Peel" , 
	L"2 Depth Peel", 
	L"3 Depth Peel", 
	L"4 Depth Peel",
	L"5 Depth Peel",
	L"6 Depth Peel"
};

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

	m_hlbDepthPeeling = CreateWindow(
		L"listbox",
		NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOTIFY, 
		700, 100, 160, 160, 
		m_hWnd, (HMENU)ID_CONTROL_TRANSPARENCY, hInstance, NULL);
	for (int i = 0; i < 7; i++)
		SendMessage(m_hlbDepthPeeling, LB_ADDSTRING, 0, (LPARAM)items[i]);

	m_DXDisplay = new WndDXDisplay(m_hInstance, m_hWnd, 30, 30, 600, 600,1);
	m_DXDisplay->ShowWindow();

	SendMessage(m_hlbDepthPeeling, LB_SETCURSEL, 0, 0);
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
		case ID_CONTROL_TRANSPARENCY:

			switch (HIWORD(wparam))
			{
				case LBN_SELCHANGE:
				{
					int iCurSel = SendMessage(m_hlbDepthPeeling, LB_GETCURSEL, 0, 0);
					SendMessage(m_DXDisplay->HWnd(), WM_COMMAND, ID_CONTROL_TRANSPARENCY, iCurSel);

					break;
				}
			}

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
