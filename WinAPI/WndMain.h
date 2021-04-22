#pragma once
#include "Window.h"

class WndDisplay;
class WndPaneModel;

class WndMain :
	public Window
{
public:
	WndMain(HINSTANCE hInstance);
	~WndMain();

	void WndProc(HWND hwnd, UINT MSG, WPARAM wparam, LPARAM lparam) override;

private:

	HWND m_hPaneScene;
	HWND m_hPaneModel;
	HWND m_hPaneAnimation;


	WndDisplay* m_DXDisplay;
};

