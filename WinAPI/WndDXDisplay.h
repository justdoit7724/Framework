#pragma once
#include "Window.h"

class StartScene;

class WndDXDisplay :
	public Window
{
public:
	WndDXDisplay(HINSTANCE hInstance, HWND parent, int x, int y, int width, int height);
	~WndDXDisplay();

	void WndProc(UINT MSG, WPARAM wparam, LPARAM lparam) override;

private:
	
	DX::Graphic* m_dxGraphic;
	StartScene* m_scene;
};

