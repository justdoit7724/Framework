#pragma once
#include "Singleton.h"

#define SGL_WND WndMgr::Instance()

class Window;

class WndMgr
{
	SET_SINGLETON(WndMgr)

public:
	void Init();
	void AddWnd(std::wstring key, const Window* wnd);
	void RemoveWnd(std::wstring key);
	void UpdateDisplays();

private:

	std::unordered_map<std::wstring, const Window*> m_mWnd;
};

