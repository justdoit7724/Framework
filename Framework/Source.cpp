#include "Window.h"
#include "Scene.h"
#include "Game_info.h"
#include "Graphic.h"
#include "Timer.h"

#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "DirectXTK.lib")

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	Window window(hInstance, "2D -> 3D");
	Graphic* graphic = new Graphic(window.Hwnd());
	Debugging::Init(graphic);
	Scene* scene = new Scene(graphic);

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
			scene->Update();
			scene->Render(graphic);

			graphic->Present();

			window.SetTitle(std::to_string(Timer::FPS()));
		}

	}

	DestroyWindow(window.Hwnd());

	return 0;
}