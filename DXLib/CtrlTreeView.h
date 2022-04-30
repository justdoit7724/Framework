#pragma once
#include "Window.h"

class CtrlTreeView :
    public WndCtrl
{
public:
	CtrlTreeView(HINSTANCE hInstance, HWND parent, int x, int y, int w, int h, int key);
	~CtrlTreeView();

	void WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) override;

	//handling items
public:
	void AddItem(std::string item);
	void DeleteItem();
	std::string GetCurItem();

	//button
public:
	void SetFuncClick(std::function<void()>&& func);
private:
	std::function<void()> m_clickFunc;
};

