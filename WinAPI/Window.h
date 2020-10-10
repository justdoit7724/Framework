#pragma once


#include "WindowDef.h"

enum class WindowType {
	Frame = WS_OVERLAPPED | WS_SYSMENU,
	Popup= WS_BORDER | WS_CHILD
};

class Window
{
public:
	Window(HINSTANCE hInstance, int x, int y, int width, int height, std::wstring className);
	Window(Window*&&);
	~Window();

	void ShowWindow();
	void AddChild(Window* CWnd);
	int GetX();
	int GetY();
	int GetWidth();
	int GetHeight();
	std::wstring GetName();

	HWND HWnd()const;
	void Size(int& w, int& h)const;

	virtual void WndProc(UINT msg, WPARAM wparam, LPARAM lparam) {};

protected:
	HWND m_hWnd;
	HINSTANCE m_hInstance;
	const std::wstring m_wstrName;

private:
	Window(const Window&) = delete;
	Window(const Window&&) = delete;
	Window(const Window*&) = delete;
	Window operator=(const Window&) = delete;
	Window operator=(const Window&&) = delete;
	Window* operator=(const Window*&) = delete;
	Window* operator=(const Window*&&) = delete;
};

