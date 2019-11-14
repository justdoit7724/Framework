

#include "Window.h"
#include "Scene.h"
#include "Game_info.h"
#include "Graphic.h"
#include "SceneMgr.h"
#include "Timer.h"
#include "TextureMgr.h"
#include "Mouse.h"

#include "Lobby.h"
#include "DebuggingScene.h"
#include "GamePlayScene.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	srand(time(NULL));
	Window window(hInstance, "Low Level");
	Graphic* graphic = new Graphic(window.Hwnd());

#ifdef _DEBUG
	SceneMgr::Instance()->Add("Debugging",new DebuggingScene());
	SceneMgr::Instance()->SetEnabled("Debugging", true);
#endif // !_DEBUG

	SceneMgr::Instance()->Add("GamePlay", new GamePlayScene());
	SceneMgr::Instance()->SetEnabled("GamePlay", true);
	SceneMgr::Instance()->Add("Lobby",new Lobby());
	SceneMgr::Instance()->SetEnabled("Lobby", true);

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

			SceneMgr::Instance()->Process(worldTimer->Elapsed(), worldTimer->SPF());

			Mouse::Instance()->Update();

			graphic->Present();

			window.SetTitle(std::to_string(worldTimer->FPS()));
		}

	}

	DestroyWindow(window.Hwnd());

	return 0;
}