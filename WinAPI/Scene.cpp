#include "stdafx.h"
#include "Scene.h"
#include "Object.h"
#include "PaneAssetScene.h"

Scene::Scene(DX::Graphic* graphic, std::string key)
	:m_dxGraphic(graphic), m_strKey(key)
{
}

Scene::~Scene()
{
}

Scene* Scene::CreateScene(SCENE_KIND kind, DX::Graphic* graphic)
{
	Scene* newScene = nullptr;

	switch (kind)
	{
		case SCENE_KIND::Play:
			break;
		case SCENE_KIND::Asset:

			newScene = new PaneAssetScene(graphic);
			break;
	}

	return newScene;
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
