#pragma once
#include "Window.h"

class PlayScene;

class WndDXDisplay :
	public Window
{
public:
	WndDXDisplay(HINSTANCE hInstance, HWND parent, int x, int y, int width, int height, int msaa);
	~WndDXDisplay();

	void WndProc(HWND hwnd, UINT MSG, WPARAM wparam, LPARAM lparam) override;

private:

	DX::Graphic* m_dxGraphic;
	PlayScene* m_scene;
	int m_iMSAA;


};

