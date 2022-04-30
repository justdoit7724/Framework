#include "stdafx.h"
#include "WndMgr.h"
#include "Window.h"

SET_SINGLETON_CPP(WndMgr, Init)


void WndMgr::Init()
{

}
void WndMgr::AddWnd(std::string key, const Window* wnd)
{
	assert(m_mWnd.find(key) == m_mWnd.end());

	m_mWnd.insert(std::pair < std::string, const Window*>(key, wnd));
}

void WndMgr::RemoveWnd(std::string key)
{
	assert(m_mWnd.find(key) != m_mWnd.end());

	m_mWnd.erase(key);
}

void WndMgr::UpdateDisplays()
{
	for (auto it = m_mWnd.begin(); it != m_mWnd.end(); ++it)
	{
		SendMessage(it->second->HWnd(), WM_COMMAND, ID_COMMAND_REALTIME_UPDATE, NULL);
	}
}