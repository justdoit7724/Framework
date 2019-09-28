#include "SceneMgr.h"
#include "Scene.h"
#include "CameraMgr.h"

void SceneMgr::Process()
{
	for (auto i = list.begin(); i != list.end(); ++i)
	{
		i->second->Logic_Update(CameraMgr::Instance()->Main());
		i->second->Render_Update(CameraMgr::Instance()->Main());
		i->second->Render();
	}
}
