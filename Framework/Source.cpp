#include "Window.h"
#include "Scene.h"
#include "Game_info.h"
#include "Graphic.h"
#include "SceneMgr.h"
#include "Timer.h"

#include "TestScene.h"
#include "DCMScene.h"
#include "DebuggingScene.h"

#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "DirectXTK.lib")

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	srand(time(NULL));
	Window window(hInstance, "Low Level");
	Graphic* graphic = new Graphic(window.Hwnd());


	TestScene* testScene = new TestScene(graphic);
	DCMScene* dcmScene = new DCMScene(graphic, testScene);
	DebuggingScene* debugScene = new DebuggingScene();

	SceneMgr::Instance()->Add(testScene);
	SceneMgr::Instance()->Add(dcmScene);
	SceneMgr::Instance()->Add(debugScene);

	Timer* worldTimer = new Timer();

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
			worldTimer->Update();

			SceneMgr::Instance()->Process();

			graphic->Present();

			window.SetTitle(std::to_string(worldTimer->FPS()));
		}

	}

	DestroyWindow(window.Hwnd());

	return 0;
}