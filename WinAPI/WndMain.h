#pragma once
#include "Window.h"
class WndMain :
	public Window
{
public:
	WndMain(HINSTANCE hInstance, int x, int y, int width, int height);

	void WndProc(HWND hwnd, UINT MSG, WPARAM wparam, LPARAM lparam) override;
	void SetComboResolution(int setID);
	void SetRadioSceneVersion(int setID);

private:
	HWND m_hRadioVisualVersion;
	HWND m_hRadioPlayVersion;
	HWND m_hComboResolution;


	Window* m_DXDisplay;
};

