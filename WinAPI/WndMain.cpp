#include "stdafx.h"
#include "WndMain.h"

#include "WndDXDisplayVisual.h"
#include "WndDXDisplayPlay.h"

DX::Graphic* g_dxGraphic=nullptr;

LRESULT WndDefaultProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

WndMain::WndMain(HINSTANCE hInstance, int x, int y, int width, int height)
	:Window(hInstance, L"main"),
	m_hRadioVisualVersion(nullptr), 
	m_hRadioPlayVersion(nullptr),
	m_hlbResolution(nullptr),

	m_DXDisplayVisual(nullptr),
	m_DXDisplayPlay(nullptr)
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

	m_hRadioVisualVersion = CreateWindow(
		L"button",
		L"Test visualization",
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP,
		700, 20, 100, 30,
		m_hWnd,
		(HMENU)ID_CONTROL_MAIN_VISUAL,
		m_hInstance,
		NULL
	);
	m_hRadioPlayVersion = CreateWindow(
		L"button",
		L"Practical play",
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
		700, 50, 100, 30,
		m_hWnd,
		(HMENU)ID_CONTROL_MAIN_PLAY,
		m_hInstance,
		NULL
	);
	m_hlbResolution = CreateWindow(
		L"listbox",
		NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOTIFY, 700, 80, 50, 85, m_hWnd, (HMENU)ID_CONTROL_RESOLUTION, m_hInstance, nullptr
	);
	SendMessage(m_hlbResolution, LB_ADDSTRING, 0, (LPARAM)L"No AA");
	SendMessage(m_hlbResolution, LB_ADDSTRING, 0, (LPARAM)L"2x");
	SendMessage(m_hlbResolution, LB_ADDSTRING, 0, (LPARAM)L"4x");
	SendMessage(m_hlbResolution, LB_ADDSTRING, 0, (LPARAM)L"8x");
	SendMessage(m_hlbResolution, LB_ADDSTRING, 0, (LPARAM)L"16x");

	m_DXDisplayVisual = new WndDXDisplayVisual(m_hInstance, m_hWnd, 30, 30, 600, 600);
	m_DXDisplayPlay = new WndDXDisplayPlay(m_hInstance, m_hWnd, 30, 30, 600, 600,1);

	SetLBResolution(0);
	SetRadioSceneVersion(ID_CONTROL_MAIN_VISUAL);
}

WndMain::~WndMain()
{
	DestroyWindow(m_hRadioVisualVersion);
	DestroyWindow(m_hRadioPlayVersion);
	DestroyWindow(m_hlbResolution);
	DestroyWindow(m_hWnd);

	delete m_DXDisplayPlay;
	delete m_DXDisplayVisual;
	SAFEDELETE(g_dxGraphic);
}

void WndMain::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_COMMAND:

		switch (LOWORD(wparam))
		{
		case ID_COMMAND_REALTIME_UPDATE:
			if(IsWindowVisible(m_DXDisplayVisual->HWnd()))
				SendMessage(m_DXDisplayVisual->HWnd(), msg, wparam, lparam);
			if (IsWindowVisible(m_DXDisplayPlay->HWnd()))
				SendMessage(m_DXDisplayPlay->HWnd(), msg, wparam, lparam);
			break;
		case ID_CONTROL_MAIN_VISUAL:
		case ID_CONTROL_MAIN_PLAY:
			SetRadioSceneVersion(wparam);
			break;
		case ID_CONTROL_RESOLUTION:
		{
			switch (HIWORD(wparam))
			{
			case LBN_SELCHANGE:
			{
				int iCurIndex = SendMessage(m_hlbResolution, LB_GETCURSEL, NULL, NULL);
				SetLBResolution(iCurIndex);
			}
			break;
			}

		}
			break;
		}
		break;
	case WM_CLOSE:

		PostQuitMessage(0);
		break;
	}
}

void WndMain::SetLBResolution(int index)
{
	SendMessage(m_hlbResolution, LB_SETCURSEL, index, NULL);

	WPARAM loword = ID_CONTROL_RESOLUTION;
	WPARAM hiword = index;
	switch (index)
	{
	case 0:
		hiword = 1;
		break;
	case 1:
		hiword = 2;
		break;
	case 2:
		hiword = 4;
		break;
	case 3:
		hiword = 8;
		break;
	case 4:
		hiword = 16;
		break;
	}
	hiword = hiword << 16;
	SendMessage(m_DXDisplayVisual->HWnd(), WM_COMMAND, loword + hiword, NULL);
	SendMessage(m_DXDisplayPlay->HWnd(), WM_COMMAND, loword + hiword, NULL);
}

void WndMain::SetRadioSceneVersion(int setID)
{

	switch (setID)
	{
	case ID_CONTROL_MAIN_VISUAL:
		SendMessage(m_hRadioPlayVersion, BM_SETCHECK, BST_UNCHECKED, NULL);
		SendMessage(m_hRadioVisualVersion, BM_SETCHECK, BST_CHECKED, NULL);
		::ShowWindow(m_DXDisplayPlay->HWnd(), SW_HIDE);
		::ShowWindow(m_DXDisplayVisual->HWnd(), SW_SHOW);
		break;
	case ID_CONTROL_MAIN_PLAY:
		SendMessage(m_hRadioPlayVersion, BM_SETCHECK, BST_CHECKED, NULL);
		SendMessage(m_hRadioVisualVersion, BM_SETCHECK, BST_UNCHECKED, NULL);
		::ShowWindow(m_DXDisplayVisual->HWnd(), SW_HIDE);
		::ShowWindow(m_DXDisplayPlay->HWnd(), SW_SHOW);
		break;
	}
}
