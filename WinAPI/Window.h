#pragma once


#include "WindowDef.h"

enum class WindowType {
	Frame,
	Child
};

/*
base

*/
class Window
{
public:
	virtual ~Window();

	//message handling
public:
	virtual void WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) = 0;
protected:
	static LRESULT CALLBACK WndDefaultProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	//creation
public:
	void RegisterWnd();
	void CreateWnd(HWND parent, DWORD dwStyle, int x, int y, int w, int h);
	const std::string m_id;
protected:
	Window(HINSTANCE hInstance, std::string id);
	const HINSTANCE m_hInstance;
	HWND m_hWnd;

	//parameters
public:
	void ShowWindow(BOOL show);
	void Focus();
	void AddChild(Window* CWnd);
	int GetX();
	int GetY();
	int GetWidth();
	int GetHeight();
	HWND HWnd()const;
	void Size(int& w, int& h)const;


	PREVENT_COPY_AND_ASSIGNMENT(Window)
};

class WndCtrl;

class WndBkg : public Window
{
public:
	WndBkg(HINSTANCE hInstance, std::string key, DWORD dwStyle, HWND parent, int x, int y, int w, int h);
	~WndBkg();

	//controls
protected:
	void AddCtrl(WndCtrl* ctrl);
private:
	std::vector<WndCtrl*> m_ctrls;
	
protected:
	void WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) override;

};

class WndCtrl : public Window
{
public:
	WndCtrl(HINSTANCE hInstance, std::string id, int key, DWORD dwStyle, HWND parent, int x, int y, int w, int h);
	~WndCtrl();

	const int m_key;

};
