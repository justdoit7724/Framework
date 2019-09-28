#include "Window.h"
#include "Scene.h"
#include "Game_info.h"
#include "Graphic.h"
#include "Timer.h"
#include "TestScene.h"
#include "SceneMgr.h"

#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "DirectXTK.lib")

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	srand(time(NULL));
	Window window(hInstance, "Low Level");
	Graphic* graphic = new Graphic(window.Hwnd());
	TestScene* testScene = new TestScene(graphic);

	Timer::Init();

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
			SceneMgr::Instance()->Process();

			graphic->Present();

			window.SetTitle(std::to_string(Timer::FPS()));
		}

	}

	DestroyWindow(window.Hwnd());

	return 0;
}