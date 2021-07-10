#pragma once
#include "Window.h"

class WndDisplay;
class CtrlButton;

class WndMain : public WndBkg
{
public:
	WndMain(HINSTANCE hInstance);
	~WndMain();

	void WndProc(HWND hwnd, UINT MSG, WPARAM wparam, LPARAM lparam) override;

	//button
private:
	void OpenWndScene();
	void OpenWndAsset();

private:

	CtrlButton* m_sceneBtn;
	CtrlButton* m_assetBtn;


	WndDisplay* m_DXDisplay;
};

