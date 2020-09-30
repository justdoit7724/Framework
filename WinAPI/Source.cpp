#include "stdafx.h"
#include "Window.h"
#include "WndDX.h"

#include "WndTest.h"

//TODO dx lib in dll vs normal project
//for now, the path to normal project was included (properties -> c/c++ path setting)

int APIENTRY main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {


	srand(time(NULL));


	const int SCN_WIDTH = 700;
	const int SCN_HEIGHT = 700;

	Window* mainWindow = new Window(hInstance, 50,50, SCN_WIDTH, SCN_HEIGHT, "Main");
	//Window* testWindow = new Window(hInstance, 600, 50, 300, 400, "Test");

	WndDX* dx = new WndDX(mainWindow->HWnd());
	mainWindow->SetWndProcPt(dx);
	mainWindow->ShowWindow();
	//testWindow->ShowWindow();
	//WndTest* test = new WndTest();
	//testWindow->SetWndProcPt(test);

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

	delete dx;

	return 0;
}

