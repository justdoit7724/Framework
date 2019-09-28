#include "SceneMgr.h"
#include "Scene.h"

void SceneMgr::Process()
{
	for (auto i = list.begin(); i != list.end(); ++i)
	{
		i->second->Update(nullptr);
	}
	for (auto i = list.begin(); i != list.end(); ++i)
	{
		i->second->Render();
	}
}
