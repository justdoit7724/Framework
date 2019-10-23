#include "SceneMgr.h"
#include "Scene.h"
#include "CameraMgr.h"

void SceneMgr::Add(Scene* scene)
{
	assert(list.find(scene->key) == list.end());

	list.insert(std::pair<std::string, Scene*>(scene->key, scene));
}

void SceneMgr::Process(float wElapsed, float wSpf)
{
	for (auto i = list.begin(); i != list.end(); ++i)
	{
		i->second->Update(wElapsed, wSpf);
		i->second->Render(CameraMgr::Instance()->Main(),0);
	}
}
