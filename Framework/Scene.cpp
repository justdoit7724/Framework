#include "Scene.h"
#include "SceneMgr.h"

Scene::Scene(std::string key)
	:key(key)
{
	SceneMgr::Instance()->Add(key, this);
}

Scene::~Scene()
{
	SceneMgr::Instance()->Remove(key);
}
