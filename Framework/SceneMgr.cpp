#include "stdafx.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "CameraMgr.h"
#include "Camera.h"

void SceneMgr::Add(std::string key, Scene* scene)
{
	executeOrder.push_back(key);

	list.insert(std::pair<std::string, Scene*>(key, scene));
}

SceneMgr::~SceneMgr()
{
	for (auto s : list)
	{
		delete s.second;
	}
}

void SceneMgr::BroadcastMessage(UINT msg)
{
	for (auto s : list)
	{
		Scene* curScene = s.second;
		
		s.second->Message(msg);
	}
}

void SceneMgr::Process(float wElapsed, float wSpf)
{
	for (auto key : executeOrder)
	{
		list[key]->Update(wElapsed, wSpf);
	}
}
