#include "stdafx.h"
#include "WndMain.h"
#include "WndMgr.h"
#include "Timer.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "AssetMgr.h"

#include "CtrlButton.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	CoInitialize(NULL);

	srand(time(NULL));

	SGL_AssetMgr.LoadModel("C:\\Users\\Jun\\Downloads\\Model\\Dragon\\fbx\\Dragon_Baked_Actions_fbx_7.4_binary.fbx");

	SGL_AssetMgr.Load();

	return 0;

	Window* mainWnd = new WndMain(hInstance);
	mainWnd->ShowWindow(TRUE);

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
			SGL_TIMER.Update();

			SGL_WND.UpdateDisplays();

			SGL_Keyboard.Update();
			SGL_Mouse.Update();
		}
	}

	delete mainWnd;
	CoUninitialize();

	return 0;
}

