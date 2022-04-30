#pragma once
#include "Singleton.h"

#define SGL_WND WndMgr::Instance()

class Window;

class WndMgr
{
	SET_SINGLETON(WndMgr)

public:
	void Init();
	void AddWnd(std::string key, const Window* wnd);
	void RemoveWnd(std::string key);
	void UpdateDisplays();

private:

	std::unordered_map<std::string, const Window*> m_mWnd;
};

