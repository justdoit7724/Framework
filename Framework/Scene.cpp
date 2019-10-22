#include "Scene.h"
#include "SceneMgr.h"
#include "Object.h"
#include "Shader.h"
#include "ShaderFormat.h"
#include "Camera.h"
#include "Debugging.h"

Scene::Scene(std::string key)
	:key(key)
{
	SceneMgr::Instance()->Add(this);
}

Scene::~Scene()
{
	SceneMgr::Instance()->Remove(key);
}

void Scene::Render_Update(const Camera* camera, float elapsed, float spf)
{
	FrustumCulling(camera);

	for (auto obj : drawObjs)
	{
		obj->Update(camera, elapsed, XMMatrixIdentity());
	}
}

void Scene::Render() const
{
	for (auto obj : drawObjs)
	{
		obj->Render();
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

	Debugging::Instance()->Draw("drawing Obj count = ", drawObjs.size(), 10, 10);
}
