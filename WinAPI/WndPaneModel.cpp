#include "stdafx.h"
#include <shobjidl.h> 
#include "WndPaneModel.h"
#include "WndDisplayModel.h"
#include "PaneModelScene.h"
#include "SceneMgr.h"

enum {
	ID_BTN_PANE_MODEL_OPEN=95,
	ID_BTN_PANE_MODEL_CLOSE,
	ID_BTN_PANE_MODEL_CVT,
	ID_LIST_PANE_MODEL_SUBMESH,
	ID_TREE_PANE_MODEL_SKELETON,
};

WndPaneModel::WndPaneModel(HINSTANCE hInstance, HWND parent)
	:Window(hInstance, L"PaneModel", WindowType::Frame, nullptr, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN))
{
	// 1. load & save
	m_hbtnClose = CreateWindow(
		L"button",L"Close",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		1400, 20, 80, 40,
		m_hWnd,
		(HMENU)ID_BTN_PANE_MODEL_CLOSE,
		hInstance, nullptr);

	m_hbtnOpen = CreateWindow(
		L"button",L"Open",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		850, 20, 80, 40,
		m_hWnd,
		(HMENU)ID_BTN_PANE_MODEL_OPEN,
		hInstance, nullptr);
	m_hbtnConvert = CreateWindow(
		L"button",L"Convert",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		950, 20, 80, 40,
		m_hWnd,
		(HMENU)ID_BTN_PANE_MODEL_CVT,
		hInstance, nullptr);

	m_heditCurModel = CreateWindow(
		L"edit", L"",
		WS_CHILD | WS_VISIBLE | ES_READONLY,
		1050, 20, 100, 40,
		m_hWnd,
		NULL,
		hInstance, nullptr);

	// 2. mesh
	m_hlistSubMesh = CreateWindow(
		L"listbox",NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOTIFY, 
		850, 130, 100, 80, 
		m_hWnd, 
		(HMENU)ID_LIST_PANE_MODEL_SUBMESH, 
		hInstance, nullptr);
	//SendMessage(m_hlistSubMesh, LB_ADDSTRING, 0, L"title");
	
	m_heditMeshInfo = CreateWindow(
		L"edit",L"정보",
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY,
		970, 130, 300, 80, 
		m_hWnd,
		(HMENU)NULL, 
		hInstance, nullptr);
	
	m_htvSkeleton = CreateWindow(
		WC_TREEVIEW, L"",
		WS_CHILD | WS_VISIBLE | TVS_HASBUTTONS | TVS_HASLINES | TVS_DISABLEDRAGDROP,
		850, 270, 150, 200,
		m_hWnd,
		(HMENU)ID_TREE_PANE_MODEL_SKELETON,
		hInstance, nullptr);
	
	m_heditSkeltonInfo = CreateWindow(
		L"edit", L"정보",
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY,
		970, 270, 300, 80,
		m_hWnd,
		(HMENU)NULL,
		hInstance, nullptr
		);
	/*
	m_hlistAnim;
	m_hstAnimInfo;
	m_htbAnimPlay;
	m_hlistMaterial;*/



	m_wndDisplay = new WndDisplayModel(hInstance, m_hWnd, 20, 20, 800, 800,1);
	m_wndDisplay->ShowWindow(TRUE);
}

WndPaneModel::~WndPaneModel()
{
	DestroyWindow(m_hbtnClose);
	DestroyWindow(m_hbtnOpen);
	DestroyWindow(m_hbtnConvert);
	DestroyWindow(m_heditCurModel);
	DestroyWindow(m_hlistSubMesh);
	DestroyWindow(m_heditMeshInfo);
	DestroyWindow(m_htvSkeleton);
	DestroyWindow(m_heditSkeltonInfo);
	DestroyWindow(m_hlistAnim);
	DestroyWindow(m_heditAnimInfo);
	DestroyWindow(m_htbAnimPlay);
	DestroyWindow(m_hlistMaterial);

	SAFEDELETE(m_scene);
	SAFEDELETE(m_wndDisplay);
}

void WndPaneModel::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_COMMAND:
	{
		switch (LOWORD(wparam))
		{
		case ID_BTN_PANE_MODEL_OPEN:
		{
			HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
				COINIT_DISABLE_OLE1DDE);
			if (SUCCEEDED(hr))
			{
				IFileOpenDialog* pFileOpen;

				// Create the FileOpenDialog object.
				hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
					IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

				if (SUCCEEDED(hr))
				{
					hr = pFileOpen->Show(NULL);

					if (SUCCEEDED(hr))
					{
						IShellItem* pItem;
						hr = pFileOpen->GetResult(&pItem);
						if (SUCCEEDED(hr))
						{
							PWSTR pszFilePath;
							hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

							// Display the file name to the user.
							if (SUCCEEDED(hr))
							{
								std::wstring strPath = pszFilePath;
								int iCutIndex = -1;
								for (int i = strPath.size() - 1; i >= 0; --i)
								{
									if (strPath[i] == '\\')
									{
										iCutIndex = i;
										break;
									}
								}
								SetWindowText(m_heditCurModel, strPath.substr(iCutIndex + 1).c_str());
								CoTaskMemFree(pszFilePath);
							}
							pItem->Release();
						}
					}
					pFileOpen->Release();
				}
				CoUninitialize();
			}
		}
			break;
		case ID_BTN_PANE_MODEL_CLOSE:
			delete this;
			break;
		case ID_BTN_PANE_MODEL_CVT:
			break;

		case ID_LIST_PANE_MODEL_SUBMESH:
		{
			switch (HIWORD(wparam))
			{
			case LBN_SELCHANGE:
				//curSel = SendMessage(hList, LB_GETCURSEL, 0, 0);
				//SendMessage(hList, LB_GETTEXT, curSel, (LPARAM)str);
				break;
			}
		}
		break;
		}
	}
	break;
	}
}

void WndPaneModel::Open(std::string modelPath)
{

}
