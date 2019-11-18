#include "Scene.h"
#include "Object.h"
#include "Shader.h"
#include "ShaderFormat.h"
#include "Camera.h"
#include "SceneMgr.h"

void Scene::Update(float elapsed, float spf)
{
	for (auto obj : objs)
	{
		if(obj->enabled)
			obj->Update();
	}
}

void Scene::Render(const Camera* camera, UINT sceneDepth) const
{
	for (auto obj : objs)
	{
		if(obj->enabled)
			obj->Render(camera, sceneDepth);
	}
}

void Scene::FrustumCulling(const Camera* camera)
{
	for (auto obj : objs)
	{
		obj->enabled = obj->IsInsideFrustum(camera->GetFrustum());
	}
}
