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
		if (curScene->Enabled())
		{
			s.second->Message(msg);
		}
	}
}

void SceneMgr::SafeDeleteScene(std::string key)
{
	assert(list.count(key));

	deleteList.push_back(key);
}

void SceneMgr::SetEnabled(std::string key, bool e)
{
	list[key]->SetEnabled(e);
}

void SceneMgr::Process(float wElapsed, float wSpf)
{
	for (auto key : executeOrder)
	{
		if (list[key]->Enabled())
		{
			list[key]->Update(wElapsed, wSpf);

			const Camera* mainCam = CameraMgr::Instance()->Main();
			list[key]->Render(mainCam->VMat()*mainCam->ProjMat(), mainCam->GetFrustum(), 0, nullptr);
		}
	}

	for (auto key : deleteList)
	{
		delete list[key];
		list.erase(key);

		for (int i = 0; i < executeOrder.size(); ++i)
		{
			if (executeOrder[i] == key)
			{
				executeOrder.erase(executeOrder.begin() + i);
				break;
			}
		}
	}
	deleteList.clear();
}
