#pragma once
#include <Windows.h>
#include <string>

class Window
{
public:
	Window(HINSTANCE _hinstance, const char* name);
	~Window();
	static LRESULT CALLBACK HandleMsgSetup(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	void SetTitle(std::string title);

	HWND Hwnd() {return hwnd;}

private:

	static LRESULT HandleMsg(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	const char* className = "DirectX2";
	HINSTANCE hInstance;
	int width;
	int height;
	HWND hwnd;
};

