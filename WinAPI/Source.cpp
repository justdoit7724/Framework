#include "stdafx.h"
#include "WndMain.h"
#include "WndDXDisplay.h"

#include "Timer.h"

int APIENTRY main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	CoInitialize(NULL);


	srand(time(NULL));

	Window* mainWnd = new WndMain(hInstance, 50,50, 1000, 700);
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

	return 0;
}

