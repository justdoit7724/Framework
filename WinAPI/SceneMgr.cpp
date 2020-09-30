
#include "stdafx.h"

#include "SceneMgr.h"
#include "Scene.h"

void SceneMgr::Add(const wchar_t* strID, Scene* scene)
{
	assert(list.find(strID) == list.end());

	list.insert(std::pair<const wchar_t*, Scene*>(strID, scene));
}

SceneMgr::SceneMgr()
{
}

SceneMgr::~SceneMgr()
{
	for (auto s : list)
	{
		delete s.second;
	}
}

void SceneMgr::BroadcastMessage(unsigned int msg)
{
	/*for (auto s : list)
	{
		Scene* curScene = s.second;

		s.second->Message(msg);
	}*/
}

void SceneMgr::Process(float wElapsed, float wSpf)
{
	for (auto key : executeOrder)
	{
		list[key]->Update(wElapsed, wSpf);
	}
}
