#include "stdafx.h"
#include "Window.h"
#include "IWindow.h"

LRESULT HandleMsg(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

Window::Window(HINSTANCE hInstance, int x, int y, int width, int height, std::string className, WindowType type)
	:m_hWnd(nullptr), m_hInstance(hInstance), m_strName(className)
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
	wstrClassName.assign(m_strName.begin(), m_strName.end());
	wc.lpszClassName = wstrClassName.c_str();
	wc.cbSize = sizeof(WNDCLASSEX);
	RegisterClassEx(&wc);

	DWORD dwStyle= WS_OVERLAPPED;
	switch (type)
	{
	case WindowType::Frame:
		dwStyle = WS_OVERLAPPED | WS_SYSMENU;
		break;
	case WindowType::Popup:
		dwStyle = WS_POPUP | WS_BORDER;
		break;
	}

	m_hWnd = CreateWindowEx(
		0,
		wstrClassName.c_str(),
		wstrClassName.c_str(),
		dwStyle,
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

	ShowWindow();
}

Window::Window(Window*&& wnd)
	:m_hWnd(wnd->m_hWnd), m_hInstance(wnd->m_hInstance), m_strName(wnd->m_strName)
{
	wnd->m_hWnd = nullptr;
}

Window::~Window()
{
	if(m_hWnd)
		DestroyWindow(m_hWnd);
}

void Window::SetWndProcPt(IWindow* iWnd)
{
	SetWindowLongPtr(m_hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(iWnd));
}

void Window::ShowWindow()
{
	::ShowWindow(m_hWnd, SW_SHOW);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);
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

std::string Window::GetName()
{
	return m_strName;
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
