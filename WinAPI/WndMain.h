#pragma once
#include "Window.h"

class WndDXDisplayVisual;
class WndDXDisplayPlay;


class WndMain :
	public Window
{
public:
	WndMain(HINSTANCE hInstance, int x, int y, int width, int height);
	~WndMain();

	void WndProc(HWND hwnd, UINT MSG, WPARAM wparam, LPARAM lparam) override;
	void SetLBResolution(int index);
	void SetRadioSceneVersion(int setID);

private:

	HWND m_hRadioVisualVersion;
	HWND m_hRadioPlayVersion;
	HWND m_hlbResolution;

	WndDXDisplayVisual* m_DXDisplayVisual;
	WndDXDisplayPlay* m_DXDisplayPlay;
};

