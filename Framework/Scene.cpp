#include "Scene.h"
#include "SceneMgr.h"
#include "Object.h"

Scene::Scene(std::string key)
	:key(key)
{
}

Scene::~Scene()
{
	SceneMgr::Instance()->Remove(key);
}

void Scene::Render_Update(const Camera* camera)
{
	for (auto obj : objs)
	{
		obj->Update(camera, XMMatrixIdentity());
	}
}

void Scene::Render() const
{
	for (auto obj : objs)
	{
		obj->Render();
	}
}
