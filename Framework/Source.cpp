#include "stdafx.h"

#include "Window.h"
#include "Scene.h"
#include "Game_info.h"
#include "Graphic.h"
#include "SceneMgr.h"
#include "Timer.h"
#include "Mouse.h"
#include "TextureMgr.h"
#include "DebuggingScene.h"
#include "PlayScene.h"
#include "RenderScene.h"
#include "Keyboard.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {



	srand(time(NULL));
	Window window(hInstance, "Low Level");
	Graphic* graphic = new Graphic(window.Hwnd());

	SceneMgr::Instance()->Add("Play", new PlayScene());

	SceneMgr::Instance()->Add("Render", new RenderScene());
	SceneMgr::Instance()->Add("Debugging", new DebuggingScene());

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

			SceneMgr::Instance()->Process(worldTimer->Elapsed(), worldTimer->SPF());

			Mouse::Instance()->Update(worldTimer->SPF());

			graphic->Present();

			window.SetTitle(std::to_string(worldTimer->FPS()));
			worldTimer->Update();

			Keyboard::Update();
		}


	}

	DestroyWindow(window.Hwnd());



	delete worldTimer;
	delete graphic;

	TextureMgr::Instance()->Release();
	SceneMgr::Instance()->Release();


	return 0;
}