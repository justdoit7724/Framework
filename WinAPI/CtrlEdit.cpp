#include "stdafx.h"
#include "CtrlEdit.h"

CtrlEdit::CtrlEdit(HINSTANCE hInstance, HWND parent, int x, int y, int w, int h,int key, BOOL readOnly)
	:WndCtrl(hInstance, "edit",key, readOnly ? (WS_CHILD | WS_VISIBLE| ES_READONLY): (WS_CHILD | WS_VISIBLE),parent,x,y,w,h),
	m_caption("")
{

}

CtrlEdit::~CtrlEdit()
{
}

void CtrlEdit::SetCaption(std::string caption)
{
	m_caption = caption;

	SetWindowTextA(m_hWnd, m_caption.c_str());
}

void CtrlEdit::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_COMMAND:
	{
		if(LOWORD(wparam)==m_key)
		{
			switch (HIWORD(wparam))
			{
			case EN_CHANGE:
				//GetWindowText(hwnd, m_caption, 256);
				break;
			}
		}
	}
	break;
	}
}
