#include "Scene.h"
#include "SceneMgr.h"
#include "Object.h"
#include "Shader.h"
#include "ShaderFormat.h"
#include "Camera.h"

Scene::Scene(std::string key)
	:key(key)
{
	SceneMgr::Instance()->Add(this);
}

Scene::~Scene()
{
	SceneMgr::Instance()->Remove(key);
}

void Scene::Update(float elapsed, float spf)
{
	for (auto obj : drawObjs)
	{
		obj->Update();
	}
}

void Scene::Render(const Camera* camera, UINT sceneDepth) const
{
	//use temp container because drawObjs is being changed in loop
	std::vector<Object*>tempMemObjs(drawObjs);

	for (auto obj : tempMemObjs)
	{
		obj->Render(camera, sceneDepth);
	}
}

void Scene::FrustumCulling(const Camera* camera)
{
	drawObjs.clear();

	for (auto obj : objs)
	{
		if(obj->IsInsideFrustum(camera->GetFrustum()))
			drawObjs.push_back(obj);
	}
}
