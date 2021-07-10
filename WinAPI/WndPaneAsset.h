#pragma once
#include "Window.h"

class WndDisplay;
class Scene;
class CtrlButton;
class CtrlEdit;
class CtrlTreeView;

class WndPaneAsset :
	public WndBkg
{
public:
	WndPaneAsset(HINSTANCE hInstance, HWND parent);
	~WndPaneAsset();

private:

	void WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)override;
	void Open(std::string modelPath);

	Scene* m_scene;

	CtrlButton* m_btnClose;
	CtrlButton* m_btnOpen;
	CtrlTreeView* m_tvProject;

	WndDisplay* m_wndDisplay;
};

