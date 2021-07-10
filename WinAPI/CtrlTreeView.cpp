#include "stdafx.h"
#include "CtrlTreeView.h"

CtrlTreeView::CtrlTreeView(HINSTANCE hInstance, HWND parent, int x, int y, int w, int h, int key)
	:WndCtrl(hInstance, WC_TREEVIEWA,key, WS_CHILD | WS_VISIBLE | TVS_HASBUTTONS | TVS_HASLINES | TVS_DISABLEDRAGDROP, parent, x,y,w,h),
	m_clickFunc(nullptr)
{
}

CtrlTreeView::~CtrlTreeView()
{
}

void CtrlTreeView::AddItem(std::string item)
{
	HTREEITEM hSelectedItem = TreeView_GetSelection(m_hWnd);

	TVINSERTSTRUCTA ti;
	ZeroMemory(&ti, sizeof(TVINSERTSTRUCT));
	ti.hParent = hSelectedItem;
	ti.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	ti.item.state = TVIS_EXPANDED;
	ti.item.stateMask = TVIS_BOLD | TVIS_CUT | TVIS_EXPANDED;
	ti.hInsertAfter = TVI_SORT;
	ti.item.iImage = 0;
	ti.item.iSelectedImage = 0;
	ti.item.pszText = (LPSTR)item.c_str();
	HTREEITEM htvitemPost = (HTREEITEM)SendMessage(m_hWnd, TVM_INSERTITEM, NULL, (LPARAM)&ti);
}

void CtrlTreeView::DeleteItem()
{
	HTREEITEM hSelectedItem = TreeView_GetSelection(m_hWnd);

	TreeView_DeleteItem(m_hWnd, hSelectedItem);
}

std::string CtrlTreeView::GetCurItem()
{
	HTREEITEM curItem = TreeView_GetSelection(m_hWnd);
	TVITEMA curItemInfo;
	curItemInfo.mask |= TVIF_TEXT;
	TreeView_GetItem((HWND)curItem, &curItemInfo);

	return curItemInfo.pszText;
}


void CtrlTreeView::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_NOTIFY:
	{
		LPNMHDR nmhdr = LPNMHDR(lparam);
		
		switch (nmhdr->code)
		{
		case TVN_SELCHANGED:
		{
			m_clickFunc();
		}
		break;
		}
	}
	break;
	}
}

void CtrlTreeView::SetFuncClick(std::function<void()>&& func)
{
	m_clickFunc = func;
}
