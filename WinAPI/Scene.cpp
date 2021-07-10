#include "stdafx.h"
#include "Scene.h"
#include "SceneMgr.h"
#include "Object.h"

Scene::Scene(DX::Graphic* graphic, std::string key)
	:m_dxGraphic(graphic), m_strKey(key)
{
	SGL_SceneMgr.Add(graphic, this);
}

Scene::~Scene()
{
	SGL_SceneMgr.Remove(m_dxGraphic, m_strKey);
}

void Scene::Update()
{
	for (auto obj : m_vRootObj)
	{
		obj->Update(m_dxGraphic);
	}
}

void Scene::Render()
{
	for (auto obj : m_vRootObj)
	{
		obj->Render(m_dxGraphic);
	}
}
