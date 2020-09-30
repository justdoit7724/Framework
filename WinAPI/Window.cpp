#include "stdafx.h"
#include "Window.h"
#include "IWindow.h"

LRESULT HandleMsg(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

Window::Window(HINSTANCE hInstance, int x, int y, int width, int height, std::string className)
{
	WNDCLASSEX wc = { 0 };
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = (WNDPROC)HandleMsg;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	std::wstring wstrClassName;
	wstrClassName.assign(className.begin(), className.end());
	wc.lpszClassName = wstrClassName.c_str();
	wc.cbSize = sizeof(WNDCLASSEX);
	RegisterClassEx(&wc);

	m_hWnd = CreateWindowEx(
		0,
		wc.lpszClassName,
		L"dx",
		WS_OVERLAPPED | WS_SYSMENU,
		x, y,
		width, height,
		NULL,
		NULL,
		hInstance,
		nullptr);


	RECT clientRect;
	GetClientRect(m_hWnd, &clientRect);
	POINT offset;
	offset.x = width - clientRect.right;
	offset.y = height - clientRect.bottom;
	MoveWindow(m_hWnd, x, y, width + offset.x, height + offset.y, FALSE);
}

Window::~Window()
{
	DestroyWindow(m_hWnd);
}

void Window::SetWndProcPt(void* pData)
{
	SetWindowLongPtr(m_hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pData));
}

void Window::ShowWindow()
{
	::ShowWindow(m_hWnd, SW_SHOW);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);
}

HWND Window::HWnd()const
{
	return m_hWnd;
}

void Window::Size(int& w, int& h)const
{
	w = 0;
	h = 0;

	RECT winRect;
	if (GetClientRect(m_hWnd, &winRect))
	{
		w = winRect.right - winRect.left;
		h = winRect.bottom - winRect.top;
	}
}

LRESULT HandleMsg(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	IWindow* iWnd = (IWindow*)(GetWindowLongPtr(hwnd, GWLP_USERDATA));

	if(iWnd)
		iWnd->WndProc(msg, wparam, lparam);

	return DefWindowProc(hwnd, msg, wparam, lparam);
}
