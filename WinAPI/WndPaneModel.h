#pragma once
#include "Window.h"

class WndDisplay;
class Scene;

class WndPaneModel :
	public Window
{
public:
	WndPaneModel(HINSTANCE hInstance, HWND parent);
	~WndPaneModel();

private:

	void WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)override;
	void Open(std::string modelPath);

	Scene* m_scene;

	HWND m_hbtnClose;
	HWND m_hbtnOpen;
	HWND m_hbtnConvert;
	HWND m_heditCurModel;
	HWND m_hlistSubMesh;
	HWND m_heditMeshInfo;
	HWND m_htvSkeleton;
	HWND m_heditSkeltonInfo;
	HWND m_hlistAnim;
	HWND m_heditAnimInfo;
	HWND m_htbAnimPlay;
	HWND m_hlistMaterial;
	WndDisplay* m_wndDisplay;
};

