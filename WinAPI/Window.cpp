#include "stdafx.h"
#include "Window.h"
#include "WndMgr.h"
#include "WndMgr.h"

LRESULT Window::WndDefaultProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	auto pWnd = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

	if (pWnd)
		pWnd->WndProc(hwnd, msg, wparam, lparam);

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

Window::Window(HINSTANCE hInstance, std::string id)
	:m_hInstance(hInstance), m_id(id), m_hWnd(nullptr)
{
}

Window::~Window()
{
	if(m_hWnd)
		DestroyWindow(m_hWnd);
}


void Window::RegisterWnd()
{
	WNDCLASSEX wc = { 0 };
	wc.style = 0;
	wc.lpfnWndProc = WndDefaultProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	std::wstring wstrId= std::wstring(m_id.begin(), m_id.end());
	wc.lpszClassName = wstrId.c_str();
	wc.cbSize = sizeof(WNDCLASSEX);
	if (!RegisterClassEx(&wc))
	{
		ASSERT_MSG(FALSE, "register window");
	}

}

void Window::CreateWnd(HWND parent, HMENU key, DWORD dwStyle, int x, int y, int w, int h)
{
	m_hWnd = CreateWindowExA(
		0,
		m_id.c_str(),
		"",
		dwStyle,
		x, y,
		w, h,
		parent,
		key,
		m_hInstance,
		nullptr);

	ASSERT_MSG(m_hWnd, "create window");
	SetWindowLongPtr(m_hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
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


WndBkg::WndBkg(HINSTANCE hInstance, std::string key, DWORD dwStyle, HWND parent, int x, int y, int w, int h)
	:Window(hInstance, key)
{
	RegisterWnd();
	CreateWnd(parent, NULL, dwStyle, x, y, w, h);

	SGL_WND.AddWnd(key, this);
}

WndBkg::~WndBkg()
{
	SGL_WND.RemoveWnd(m_id);

	for (auto ctrl : m_ctrls)
	{
		delete ctrl;
	}
}

void WndBkg::AddCtrl(WndCtrl* ctrl)
{
	if (!ctrl)
		return; 

	m_ctrls.push_back(ctrl);
}

void WndBkg::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	for (auto ctrl : m_ctrls)
	{
		ctrl->WndProc(hwnd, msg, wparam, lparam);
	}
}


WndCtrl::WndCtrl(HINSTANCE hInstance, std::string id, int key, DWORD dwStyle, HWND parent, int x, int y, int w, int h)
	:Window(hInstance, id), m_key(key)
{
	CreateWnd(parent, (HMENU)key, dwStyle, x, y, w, h);
}

WndCtrl::~WndCtrl()
{
}

