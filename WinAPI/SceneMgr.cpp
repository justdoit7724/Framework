#include "stdafx.h"
#include "SceneMgr.h"
#include "Scene.h"

SET_SINGLETON_CPP(SceneMgr, Init)

void SceneMgr::Add(DX::Graphic* graphic, Scene* scene)
{
	if (m_scenes[graphic->m_id].find(scene->m_strKey) != m_scenes[graphic->m_id].end())
		return;

	m_scenes[graphic->m_id][scene->m_strKey]= scene;
}
void SceneMgr::Remove(DX::Graphic* graphic, std::string key)
{
	if (m_scenes[graphic->m_id].find(key) == m_scenes[graphic->m_id].end())
		return;

	m_scenes[graphic->m_id].erase(key);
}

void SceneMgr::Update(DX::Graphic* graphic)
{
	for (auto scene : m_scenes[graphic->m_id])
	{
		scene.second->Update();
	}
}

void SceneMgr::Render(DX::Graphic* graphic)
{
	for (auto scene : m_scenes[graphic->m_id])
	{
		scene.second->Render();
	}
}

void SceneMgr::Init()
{
}
