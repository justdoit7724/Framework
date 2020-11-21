#pragma once
#include "Window.h"

class PlayScene;

class WndDXDisplayPlay :
	public Window
{
public:
	WndDXDisplayPlay(HINSTANCE hInstance, HWND parent, int x, int y, int width, int height, int msaa);
	~WndDXDisplayPlay();

	void WndProc(HWND hwnd, UINT MSG, WPARAM wparam, LPARAM lparam) override;

private:

	PlayScene* m_scene;
	int m_iMSAA;
};

