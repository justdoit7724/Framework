
#include "SceneMgr.h"
#include "Scene.h"
#include "CameraMgr.h"
#include "Camera.h"
#include "SetupScene.h"

void SceneMgr::Add(std::string key, Scene* scene)
{
	executeOrder.push_back(key);

	list.insert(std::pair<std::string, Scene*>(key, scene));
}

SceneMgr::SceneMgr()
{
	Add("Setup", new SetupScene());
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
