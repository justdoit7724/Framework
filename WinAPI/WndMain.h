#pragma once
#include "Window.h"
class WndMain :
	public Window
{
public:
	WndMain(HINSTANCE hInstance, int x, int y, int width, int height);

	void WndProc(UINT MSG, WPARAM wparam, LPARAM lparam) override;
	void SetCheckBoxLight(int setID, bool bCheck);

private:
	HWND m_hCheckDLight;
	HWND m_hCheckPLight;
	HWND m_hCheckSLight;

	Window* m_DXDisplay;
};

