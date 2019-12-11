#include "stdafx.h"
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

void Scene::Render(const XMMATRIX& vp, const Frustum& frustum, UINT sceneDepth, void const* subject) const
{
	for (auto obj : objs)
	{
		if (subject == obj)
			continue;

		obj->Render(vp, frustum,sceneDepth);
	}
}

void Scene::GetObjs(std::vector<const Object*>& rObj)const
{
	for (auto o : objs)
	{
		rObj.push_back(o);
	}
}

void Scene::AddObj(Object* obj)
{
	objs.push_back(obj);
}
