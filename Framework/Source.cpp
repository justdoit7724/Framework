

#include "Window.h"
#include "Scene.h"
#include "Game_info.h"
#include "Graphic.h"
#include "SceneMgr.h"
#include "Timer.h"
#include "TextureMgr.h"

#include "TestScene.h"
#include "DCMScene.h"
#include "DebuggingScene.h"
#include "BindingTestScene.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	srand(time(NULL));
	Window window(hInstance, "Low Level");
	Graphic* graphic = new Graphic(window.Hwnd());

	TextureMgr::Instance()->Load(KEY_TEXTURE_NORMAL_DEFAULT, FN_TEXTURE_NORMAL_DEFAULT);

#ifdef _DEBUG
	Scene* debugScene = new DebuggingScene();
#endif // !_DEBUG
	Scene* testScene = new TestScene(graphic);
	//Scene* dcmScene = new DCMScene(graphic, testScene);
	//Scene* bindScene = new BindingTestScene();



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

			graphic->Present();

			window.SetTitle(std::to_string(worldTimer->FPS()));
		}

	}

	DestroyWindow(window.Hwnd());

	return 0;
}