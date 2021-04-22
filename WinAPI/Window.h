#pragma once


#include "WindowDef.h"

enum class WindowType {
	Frame,
	Child
};

class Window
{
public:
	Window(HINSTANCE hInstance, std::wstring className, WindowType wndType, HWND parent, int x, int y, int width, int height);
	virtual ~Window();

	void ShowWindow(BOOL show);
	void Focus();
	void AddChild(Window* CWnd);
	int GetX();
	int GetY();
	int GetWidth();
	int GetHeight();
	std::wstring GetName();

	HWND HWnd()const;
	void Size(int& w, int& h)const;

	virtual void WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {};

	const std::wstring m_wstrName;

protected:
	HWND m_hWnd;
	HINSTANCE m_hInstance;

private:
	Window(const Window&) = delete;
	Window(const Window&&) = delete;
	Window(const Window*&) = delete;
	Window operator=(const Window&) = delete;
	Window operator=(const Window&&) = delete;
	Window* operator=(const Window*&) = delete;
	Window* operator=(const Window*&&) = delete;
};


