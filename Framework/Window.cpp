#include "Window.h"
#include "Game_info.h"
#include "Mouse.h"
#include "Keyboard.h"

Window::Window(HINSTANCE _hinstance, const char* name)
{
	hInstance = _hinstance;

	WNDCLASSEX wc = { 0 };
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = className;
	wc.cbSize = sizeof(WNDCLASSEX);
	RegisterClassEx(&wc);

	/*RECT wr;
	wr.left = 100;
	wr.right = wr.left + _width;
	wr.top = 100;
	wr.bottom = wr.top + _height;
	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);*/

	hwnd = CreateWindowEx(
		0,
		className,
		name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		0, 0,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		this);

	ShowWindow(hwnd, SW_SHOWDEFAULT);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);
}


Window::~Window()
{
	DestroyWindow(hwnd);
}

LRESULT Window::HandleMsgSetup(HWND _hwnd, UINT _msg, WPARAM _wparam, LPARAM _lparam)
{
	if (_msg == WM_NCCREATE) {
		
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(_lparam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		SetWindowLongPtr(_hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		SetWindowLongPtr(_hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsg));
	}

	return DefWindowProc(_hwnd, _msg, _wparam, _lparam);
}

void Window::SetTitle(std::string title)
{
	SetWindowText(hwnd, title.c_str());
}

LRESULT Window::HandleMsg(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg) {

	case WM_KEYUP:
		Keyboard::Release(static_cast<char>(wparam));
		return 0;
	case WM_LBUTTONDOWN:
		Mouse::UpdateLeft(true);
		return 0;
	case WM_LBUTTONUP:
		Mouse::UpdateLeft(false);
		return 0;
	case WM_RBUTTONDOWN:
		Mouse::UpdateRight(true);
		return 0;
	case WM_RBUTTONUP:
		Mouse::UpdateRight(false);
		return 0;
	case WM_KEYDOWN:
	{
		if (static_cast<char>(wparam) == VK_ESCAPE)
		{
			//DestroyWindow(hwnd);
			PostQuitMessage(0);
			return 0;
		}

		Keyboard::Press(wparam);
	}
		return 0;

	case WM_MOUSEMOVE:
		Mouse::UpdatePt(lparam);
		return 0;




	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

	default:
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
}
