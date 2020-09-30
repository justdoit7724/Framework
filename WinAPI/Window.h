#pragma once

class IWindow;

class Window
{
public:
	Window(HINSTANCE hInstance, int x, int y, int width, int height, std::string className);
	~Window();

	void SetWndProcPt(void* pData);
	void ShowWindow();

	HWND HWnd()const;
	void Size(int& w, int& h)const;

private:
	HWND m_hWnd;
};

