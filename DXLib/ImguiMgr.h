#pragma once

#include "SDK/imgui/imgui.h"

namespace DX
{
	class Graphic;

	DXLIB_DLL void IMGUI_Init(Graphic* graphic);
	DXLIB_DLL void IMGUI_Release();
	DXLIB_DLL void IMGUI_StartFrame();
	DXLIB_DLL void IMGUI_EndFrame();
	DXLIB_DLL LRESULT IMGUI_Wndproc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
}
