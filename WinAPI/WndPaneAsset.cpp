#include "stdafx.h"
#include "WndPaneAsset.h"
#include "WndDisplayAsset.h"
#include "PaneAssetScene.h"
#include "SceneMgr.h"
#include "FileCtl.h"
#include "XReader.h"
#include "CtrlButton.h"
#include "CtrlEdit.h"
#include "CtrlTreeView.h"

enum {
	ID_BTN_PANE_ASSET_OPEN=95,
	ID_BTN_PANE_ASSET_CLOSE,
	ID_BTN_PANE_ASSET_PROJECT,
};

WndPaneAsset::WndPaneAsset(HINSTANCE hInstance, HWND parent)
	:WndBkg(hInstance, "PaneModel", WS_MAXIMIZE | WS_POPUP, nullptr, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN))
{
	// 1. load & save
	m_btnClose = new CtrlButton(hInstance, m_hWnd, "Close",1400, 20, 80, 40, ID_BTN_PANE_ASSET_OPEN);
	m_btnOpen = new CtrlButton(hInstance, m_hWnd, "Open", 850, 20, 80, 40, ID_BTN_PANE_ASSET_CLOSE);
	
	m_tvProject = new CtrlTreeView(hInstance, m_hWnd, 50, 700, 200, 400, ID_BTN_PANE_ASSET_PROJECT);

	m_wndDisplay = new WndDisplayAsset(hInstance, m_hWnd, 20, 20, 800, 800,1);
	m_wndDisplay->ShowWindow(TRUE);
}

WndPaneAsset::~WndPaneAsset()
{
	SAFEDELETE(m_btnClose);
	SAFEDELETE(m_btnOpen);
	SAFEDELETE(m_tvProject);

	SAFEDELETE(m_wndDisplay);
}

void WndPaneAsset::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
}

void WndPaneAsset::Open(std::string modelPath)
{

}
