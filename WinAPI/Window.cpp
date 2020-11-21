#include "stdafx.h"
#include "Window.h"

LRESULT CALLBACK WndDefaultProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

Window::Window(HINSTANCE hInstance, int x, int y, int width, int height, std::wstring className)
	:m_hWnd(nullptr), m_hInstance(hInstance), m_wstrName(className)
{
	WNDCLASSEX wc = { 0 };
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndDefaultProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_wstrName.c_str();
	wc.cbSize = sizeof(WNDCLASSEX);
	RegisterClassEx(&wc);
}

Window::Window(Window*&& wnd)
	:m_hWnd(wnd->m_hWnd), m_hInstance(wnd->m_hInstance), m_wstrName(wnd->m_wstrName)
{
	wnd->m_hWnd = nullptr;
}

Window::~Window()
{
	if(m_hWnd)
		DestroyWindow(m_hWnd);
}

void Window::ShowWindow()
{
	::ShowWindow(m_hWnd, SW_SHOW);
	/*SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);*/
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

LRESULT CALLBACK WndDefaultProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (msg == WM_NCCREATE)
	{
 		Window* pWnd = (Window*)((LPCREATESTRUCT)lparam)->lpCreateParams;
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
	}

	auto pWnd = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	if (pWnd)
		pWnd->WndProc(msg, wparam, lparam);

	return DefWindowProc(hwnd, msg, wparam, lparam);
}