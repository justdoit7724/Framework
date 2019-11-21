#include "Scene.h"
#include "Object.h"
#include "Shader.h"
#include "ShaderFormat.h"
#include "Camera.h"
#include "Transform.h"
#include "SceneMgr.h"

Scene::~Scene()
{
	for (auto obj : objs)
		delete obj;
}

void Scene::Update(float elapsed, float spf)
{
	for (auto obj : objs)
	{
		obj->Update();
	}
}

void Scene::Render(const Camera* camera, UINT sceneDepth) const
{
	for (auto obj : objs)
	{
		if (obj->IsInsideFrustum(camera->GetFrustum()))
		{
			XMMATRIX vp = camera->VMat() * camera->ProjMat(obj->zOrder);

			obj->Render(vp, camera->transform->GetPos(), sceneDepth);
		}
	}
}

const Object* Scene::GetObj(UINT id)
{
	if (objs.size() <= id)
		return nullptr;

	return objs[id];
}