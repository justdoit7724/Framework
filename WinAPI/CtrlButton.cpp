#include "stdafx.h"
#include "CtrlButton.h"

CtrlButton::CtrlButton(HINSTANCE hInstance, HWND parent, std::string caption, int x, int y, int w, int h, int key)
	:WndCtrl(hInstance, "button",m_key, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, parent, x,y,w,h),
	m_caption(""),
	m_func(nullptr)
{
}

CtrlButton::~CtrlButton()
{
}

void CtrlButton::SetCaption(std::string caption)
{
	m_caption = caption;
}

void CtrlButton::SetClickFunc(std::function<void()>&& func)
{
	m_func.swap(func);
}

void CtrlButton::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_COMMAND:
	{
		if (LOWORD(wparam) == m_key)
		{
			if(m_func)
				m_func();
		}
	}
	break;
	}
}
