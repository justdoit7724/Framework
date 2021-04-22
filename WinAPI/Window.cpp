#include "stdafx.h"
#include "Window.h"
#include "WndMgr.h"

LRESULT CALLBACK WndDefaultProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

Window::Window(HINSTANCE hInstance, std::wstring className, WindowType wndType, HWND parent, int x, int y, int width, int height)
	:m_hWnd(nullptr), m_hInstance(hInstance), m_wstrName(className)
{

	// only register here not create window
	// for getting create message in inherited class
	WNDCLASSEX wc = { 0 };
	wc.style = 0;
	wc.lpfnWndProc = WndDefaultProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_wstrName.c_str();
	wc.cbSize = sizeof(WNDCLASSEX);
	RegisterClassEx(&wc);
	

	DWORD dwStyle = 0;
	HWND hParent = nullptr;
	switch (wndType)
	{
	case WindowType::Frame:
		dwStyle = WS_MAXIMIZE | WS_POPUP;
		break;
	case WindowType::Child:
		dwStyle = WS_CHILD;
		hParent = parent;
		break;
	}
	m_hWnd = CreateWindowEx(
		0,
		m_wstrName.c_str(),
		m_wstrName.c_str(),
		dwStyle,
		x, y,
		width, height,
		hParent,
		NULL,
		hInstance,
		nullptr);
	SetWindowLongPtr(m_hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	SGL_WND.AddWnd(m_wstrName, this);
}


Window::~Window()
{
	DestroyWindow(m_hWnd);

	SGL_WND.RemoveWnd(m_wstrName);
}

LRESULT CALLBACK WndDefaultProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	auto pWnd = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

	if (pWnd)
		pWnd->WndProc(hwnd, msg, wparam, lparam);

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void Window::ShowWindow(BOOL show)
{
	::ShowWindow(m_hWnd, show?SW_SHOW:SW_HIDE);
	::EnableWindow(m_hWnd, show);
	/*SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);*/
}

void Window::Focus()
{
	::SetFocus(m_hWnd);
}

void Window::AddChild(Window* CWnd)
{
	SetParent(CWnd->m_hWnd, m_hWnd);
}

int Window::GetX()
{
	RECT rc;

	if (GetClientRect(m_hWnd, &rc))
		return rc.left;

	return -1;
}

int Window::GetY()
{
	RECT rc;

	if (GetClientRect(m_hWnd, &rc))
		return rc.top;

	return -1;
}

int Window::GetWidth()
{
	RECT rc;

	if (GetClientRect(m_hWnd, &rc))
		return (rc.right-rc.left);

	return -1;
}

int Window::GetHeight()
{
	RECT rc;

	if (GetClientRect(m_hWnd, &rc))
		return (rc.bottom - rc.top);

	return -1;
}

std::wstring Window::GetName()
{
	return m_wstrName;
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