#pragma once
#include "Window.h"

class Scene;

class WndDisplay :
	public Window
{
public:
	WndDisplay(HINSTANCE hInstance, HWND parent, int x, int y, int width, int height, int msaa);
	virtual ~WndDisplay();

	void WndProc(HWND hwnd, UINT MSG, WPARAM wparam, LPARAM lparam) override;
	BOOL AddScene(Scene* scene);

protected:

	DX::Graphic* m_dxGraphic;
	int m_iMSAA;

};

