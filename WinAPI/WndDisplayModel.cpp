#include "stdafx.h"
#include "WndDisplayModel.h"
#include "PaneModelScene.h"
#include "SceneMgr.h"

WndDisplayModel::WndDisplayModel(HINSTANCE hInstance, HWND parent, int x, int y, int width, int height, int msaa)
	:WndDisplay(hInstance, parent, x, y, width, height, msaa)
{
	SGL_SceneMgr.Add(m_dxGraphic, new PaneModelScene(m_dxGraphic));
}
