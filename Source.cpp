
#define NOMINMAX
#include <Windows.h>
#include <time.h>
#include <string>
#include "DXFramework\DXFramework.h"

LRESULT HandleMsgSetup(HWND _hwnd, UINT _msg, WPARAM _wparam, LPARAM _lparam);
LRESULT HandleMsg(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	const int SCN_WIDTH = 700;
	const int SCN_HEIGHT = 700;

	DXFramework* dx = nullptr;

#pragma region create window

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
	wc.lpszClassName = L"winAPI";
	wc.cbSize = sizeof(WNDCLASSEX);
	RegisterClassEx(&wc);

	UINT x = 10;
	UINT y = 10;

	HWND hwnd = CreateWindowEx(
		0,
		wc.lpszClassName,
		L"dx",
		WS_OVERLAPPED | WS_SYSMENU,
		x, y,
		SCN_WIDTH, SCN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		dx);

	ShowWindow(hwnd, SW_SHOW);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	RECT clientRect;
	GetClientRect(hwnd, &clientRect);
	POINT offset;
	offset.x = SCN_WIDTH - clientRect.right;
	offset.y = SCN_HEIGHT - clientRect.bottom;
	MoveWindow(hwnd, x, y, SCN_WIDTH + offset.x, SCN_HEIGHT + offset.y, FALSE);

#pragma endregion


	srand(time(NULL));

	dx = new DXFramework(hwnd, SCN_WIDTH, SCN_HEIGHT);

#pragma region loop operation

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (msg.message != WM_QUIT) {

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

		}
		else
		{
			dx->Update();
		}


	}
#pragma endregion


	DestroyWindow(hwnd);

	delete dx;

	return 0;
}

LRESULT HandleMsgSetup(HWND _hwnd, UINT _msg, WPARAM _wparam, LPARAM _lparam)
{
	if (_msg == WM_NCCREATE) {

		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(_lparam);
		DXFramework* const dx = static_cast<DXFramework*>(pCreate->lpCreateParams);
		SetWindowLongPtr(_hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(dx));
		SetWindowLongPtr(_hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&HandleMsg));
	}

	return DefWindowProc(_hwnd, _msg, _wparam, _lparam);
}


LRESULT HandleMsg(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	DXFramework* dx = (DXFramework*)(GetWindowLongPtr(hwnd, GWLP_USERDATA));

	dx->OnWndProc(msg, wparam, lparam);

	switch (msg)
	{
	case WM_DESTROY:
		exit(-1);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}
