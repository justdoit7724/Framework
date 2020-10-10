#pragma once
#include "Window.h"
class WndMain :
	public Window
{
public:
	WndMain(HINSTANCE hInstance, int x, int y, int width, int height);

	void WndProc(UINT MSG, WPARAM wparam, LPARAM lparam) override;
	void SetRadioLight(int setID);

private:
	HWND m_hRadioDLight;
	HWND m_hRadioPLight;
	HWND m_hRadioSLight;

	Window* m_DXDisplay;
};

