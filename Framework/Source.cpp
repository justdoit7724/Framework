

#include "Window.h"
#include "Scene.h"
#include "Game_info.h"
#include "Graphic.h"
#include "SceneMgr.h"
#include "Timer.h"
#include "TextureMgr.h"
#include "Mouse.h"
#include "Debugging.h"

#include "Lobby.h"
#include "DebuggingScene.h"
#include "GamePlayScene.h"
#include "CameraMgr.h"
#include "Keyboard.h"
#include <dxgidebug.h>

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	srand(time(NULL));
	Window window(hInstance, "Low Level");
	Graphic* graphic = new Graphic(window.Hwnd());


#ifdef _DEBUG
	Scene* debugScene = new DebuggingScene();
	SceneMgr::Instance()->Add("Debugging", debugScene);
	SceneMgr::Instance()->SetEnabled("Debugging", true);
#endif // !_DEBUG

	Scene* playScene = new GamePlayScene();
	SceneMgr::Instance()->Add("GamePlay", playScene);
	SceneMgr::Instance()->SetEnabled("GamePlay", true);
	Scene* lobbyScene = new Lobby();
	SceneMgr::Instance()->Add("Lobby", lobbyScene);
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



	delete worldTimer;
	delete graphic;

	TextureMgr::Instance()->Release();
	SceneMgr::Instance()->Release();


	ID3D11Debug* debug;
	r_assert(DX_Device->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&debug)));
	debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);

	return 0;
}