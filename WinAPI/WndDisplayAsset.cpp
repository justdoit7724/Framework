#include "stdafx.h"
#include "WndDisplayAsset.h"
#include "PaneAssetScene.h"
#include "SceneMgr.h"

WndDisplayAsset::WndDisplayAsset(HINSTANCE hInstance, HWND parent, int x, int y, int width, int height, int msaa)
	:WndDisplay(hInstance, parent, x, y, width, height, msaa)
{
	SGL_SceneMgr.Add(m_dxGraphic, new PaneAssetScene(m_dxGraphic));
}
