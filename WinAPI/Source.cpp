#include "stdafx.h"
#include "WndMain.h"

#include "Timer.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	CoInitialize(NULL);

	srand(time(NULL));

	double dScnSizeX = GetSystemMetrics(SM_CXSCREEN);
	double dScnSizeY = GetSystemMetrics(SM_CYSCREEN);
	Window* mainWnd = new WndMain(hInstance, 0,0, dScnSizeX, dScnSizeY);
	mainWnd->ShowWindow();

	Timer timer;

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
			timer.Update();

			SendMessage(mainWnd->HWnd(), WM_COMMAND, ID_COMMAND_REALTIME_UPDATE, (LPARAM)&timer);
		}
	}

	delete mainWnd;
	CoUninitialize();

	return 0;
}

