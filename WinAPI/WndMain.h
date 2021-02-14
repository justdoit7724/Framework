#pragma once
#include "Window.h"

class WndDXDisplay;


class WndMain :
	public Window
{
public:
	WndMain(HINSTANCE hInstance, int x, int y, int width, int height);
	~WndMain();

	void WndProc(HWND hwnd, UINT MSG, WPARAM wparam, LPARAM lparam) override;

private:

	WndDXDisplay* m_DXDisplay;
};

