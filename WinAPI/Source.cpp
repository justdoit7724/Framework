#include "stdafx.h"
#include "WndMain.h"
#include "WndMgr.h"
#include "Timer.h"
#include "Keyboard.h"
#include "Mouse.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	CoInitialize(NULL);

	srand(time(NULL));

	Window* mainWnd = new WndMain(hInstance);
	mainWnd->ShowWindow(TRUE);

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
			SGL_TIMER.Update();

			SGL_WND.UpdateDisplays();

			SGL_Keyboard.Update();
			SGL_Mouse.Update();
		}
	}

	delete mainWnd;
	CoUninitialize();

	return 0;
}

