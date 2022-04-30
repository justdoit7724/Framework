#pragma once
#include "Window.h"
class CtrlEdit :public WndCtrl
{
public:
	CtrlEdit(HINSTANCE hInstance, HWND parent, int x, int y, int w, int h, int key, BOOL readOnly);
	~CtrlEdit();

	void SetCaption(std::string caption);
	void WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
	std::string m_caption;
};

