#pragma once
#include "Window.h"

class Scene;
class VisualAAScene;
class PlayScene;

class WndDXDisplay :
	public Window
{
public:
	WndDXDisplay(HINSTANCE hInstance, HWND parent, int x, int y, int width, int height);
	~WndDXDisplay();

	void WndProc(HWND hwnd, UINT MSG, WPARAM wparam, LPARAM lparam) override;

private:
	
	DX::Graphic* m_dxGraphic;
	VisualAAScene* m_visualAAScene;
	PlayScene* m_playScene;
	Scene* m_curScene;
};

