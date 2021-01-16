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
	
	m_hgbCurPeel = CreateWindow(
		L"button",
		L"Peeling Layer",
		WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
		700, 275, 110, 60,
		m_hWnd,
		(HMENU)0,
		hInstance,
		NULL
	);

	m_DXDisplay = new WndDXDisplay(m_hInstance, m_hWnd, 30, 30, 600, 600,1);
	m_DXDisplay->ShowWindow();

	SendMessage(m_hlbDepthPeeling, LB_SETCURSEL, 0, 0);

}

WndMain::~WndMain()
{
	delete m_DXDisplay;
	DestroyWindow(m_hlbDepthPeeling);
	DestroyWindow(m_hWnd);
	DestroyWindow(m_hgbCurPeel);
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
			{
				SendMessage(m_DXDisplay->HWnd(), msg, wparam, lparam);

				RECT roi;
				roi.left = 700;
				roi.top = 300;
				roi.right = roi.left + 110;
				roi.bottom = roi.top + 60;
				InvalidateRect(hwnd, &roi, TRUE);
			}
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
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC dc = BeginPaint(hwnd, &ps);

		int iCurSel = SendMessage(m_hlbDepthPeeling, LB_GETCURSEL, NULL, NULL);

		int iPeelingIndex = m_DXDisplay->GetPeelIndex();

		std::string strPeelingIndex = "None";
		if (iCurSel != 0)
		{
			strPeelingIndex = std::to_string(iPeelingIndex);

			if (iPeelingIndex == -1)
			{
				strPeelingIndex = "All";
			}
		}

		SetBkColor(dc, RGB(240, 240, 240));
		TextOutA(dc, 720, 300, strPeelingIndex.c_str(), strPeelingIndex.size());

		TextOutA(dc, 850, 300, "Mouse wheel", 11);

		EndPaint(hwnd, &ps);
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
