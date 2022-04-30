#include "pch.h"
#include "ImguiMgr.h"
#include "SDK/imgui/backends/imgui_impl_win32.h"
#include "SDK/imgui/backends/imgui_impl_dx11.h"
#include "Graphic.h"


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


DXLIB_DLL void DX::IMGUI_Init(Graphic* graphic)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(graphic->GetHWND());
	ImGui_ImplDX11_Init(graphic->Device(), graphic->DContext());
}

DXLIB_DLL void DX::IMGUI_Release()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

DXLIB_DLL void DX::IMGUI_StartFrame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

DXLIB_DLL void DX::IMGUI_EndFrame()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

DXLIB_DLL LRESULT DX::IMGUI_Wndproc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
}
