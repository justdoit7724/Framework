#pragma once
#include "Window.h"

class CtrlButton : public WndCtrl
{
public:
	CtrlButton(HINSTANCE hInstance, HWND parent, std::string caption, int x, int y, int w, int h, int key);
	~CtrlButton();

	void SetCaption(std::string caption);
	void SetClickFunc(std::function<void()>&& func);
	void WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)override;
private:
	std::function<void()> m_func;
	std::string m_caption;
};

