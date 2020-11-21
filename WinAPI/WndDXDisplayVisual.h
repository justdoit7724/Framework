#pragma once
#include "Window.h"

class VisualAAScene;

class WndDXDisplayVisual :
	public Window
{
public:
	WndDXDisplayVisual(HINSTANCE hInstance, HWND parent, int x, int y, int width, int height);
	~WndDXDisplayVisual();

	void WndProc(HWND hwnd, UINT MSG, WPARAM wparam, LPARAM lparam) override;

private:

	DX::Graphic* m_graphic;
	VisualAAScene* m_scene;
	int m_iMSAA;
};

