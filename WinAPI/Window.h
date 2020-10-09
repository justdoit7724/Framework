#pragma once

class IWindow;

enum class WindowType {
	Frame,
	Popup
};

class Window
{
public:
	Window(HINSTANCE hInstance, int x, int y, int width, int height, std::string className, WindowType type);
	Window(Window*&&);
	~Window();

	void SetWndProcPt(IWindow* iWnd);
	void ShowWindow();
	void AddChild(Window* CWnd);
	int GetX();
	int GetY();
	int GetWidth();
	int GetHeight();
	std::string GetName();

	HWND HWnd()const;
	void Size(int& w, int& h)const;



private:
	HWND m_hWnd;
	HINSTANCE m_hInstance;
	const std::string m_strName;

	Window(const Window&) = delete;
	Window(const Window&&) = delete;
	Window(const Window*&) = delete;
	Window operator=(const Window&) = delete;
	Window operator=(const Window&&) = delete;
	Window* operator=(const Window*&) = delete;
	Window* operator=(const Window*&&) = delete;
};

