#include "stdafx.h"
#include "Window.h"
#include "WndDX.h"

#include "WndTest.h"

//TODO dx lib in dll vs normal project
//for now, the path to normal project was included (properties -> c/c++ path setting)

int APIENTRY main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	CoInitialize(NULL);


	srand(time(NULL));

	Window* mainWnd = new Window(hInstance, 50,50, 1000, 700, "Main", WindowType::Frame);
	Window* dxWnd = new Window(hInstance, 50,50, 600, 600, "DX_display", WindowType::Popup);
	mainWnd->AddChild(std::move(dxWnd));

	WndDX* dx = new WndDX(dxWnd->HWnd());
	dxWnd->SetWndProcPt(dx);

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

	delete dx;

	return 0;
}

