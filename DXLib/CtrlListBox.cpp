#include "stdafx.h"
#include "CtrlListBox.h"

CtrlListBox::CtrlListBox(HINSTANCE hInstance, HWND parent, int x, int y, int w, int h, int key)
	:WndCtrl(hInstance, "listbox",key, WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOTIFY, parent, x,y,w,h)
{
}

CtrlListBox::~CtrlListBox()
{
}

void CtrlListBox::AddItem(std::string title)
{
	SendMessage(m_hWnd, LB_ADDSTRING, 0, (LPARAM)title.c_str());
}

void CtrlListBox::DeleteItem(int index)
{
	SendMessage(m_hWnd, LB_DELETESTRING, index, NULL);
}

void CtrlListBox::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg)
	{
	case WM_COMMAND:
	{
		if (LOWORD(wparam) == m_key)
		{
			switch (HIWORD(wparam))
			{
			case LBN_SELCHANGE:
			{
				int curSel = SendMessage(m_hWnd, LB_GETCURSEL, 0, 0);
				wchar_t str[256];
				SendMessage(m_hWnd, LB_GETTEXT, curSel, (LPARAM)str);
			}
				break;
			}
		}
	}
	break;
	}
}
