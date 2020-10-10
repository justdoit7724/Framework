#pragma once
#include "Window.h"

class Scene;

class WndDXDisplay :
	public Window
{
public:
	WndDXDisplay(HINSTANCE hInstance, HWND parent, int x, int y, int width, int height);
	~WndDXDisplay();

	void WndProc(UINT MSG, WPARAM wparam, LPARAM lparam) override;

private:
	
	DX::Graphic* m_dxGraphic;
	Scene* m_scene;
};

