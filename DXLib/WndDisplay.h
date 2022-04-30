#pragma once
#include "Window.h"
#include "DefComponent.h"

class Scene;

class WndDisplay :
	public Window
{
public:
	WndDisplay(HINSTANCE hInstance, HWND parent, int x, int y, int width, int height, int msaa);
	virtual ~WndDisplay();

	void WndProc(HWND hwnd, UINT MSG, WPARAM wparam, LPARAM lparam) override;
	BOOL AddScene(SCENE_KIND kind);

protected:

	DX::Graphic* m_dxGraphic;
	int m_iMSAA;

	std::vector<Scene*> m_scenes;
};

