#include "SceneMgr.h"
#include "Scene.h"
#include "CameraMgr.h"

void SceneMgr::Add(Scene* scene)
{
	assert(list.find(scene->key) == list.end());

	list.insert(std::pair<std::string, Scene*>(scene->key, scene));
}

void SceneMgr::Process()
{
	for (auto i = list.begin(); i != list.end(); ++i)
	{
		i->second->Logic_Update();
		i->second->Render_Update(CameraMgr::Instance()->Main());
		i->second->Render();
	}
}
