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

void Scene::Render(const XMMATRIX& vp, const Frustum& frustum, UINT sceneDepth) const
{
	for (auto obj : objs)
	{
		if (obj->IsInsideFrustum(frustum))
		{
			obj->Render(XMMatrixIdentity(), vp, sceneDepth);
		}
	}
}

const Object* Scene::GetObj(UINT id)const
{
	if (objs.size() <= id)
		return nullptr;

	return objs[id];
}