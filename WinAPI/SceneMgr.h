#pragma once
#include "Singleton.h"

class Scene;

class SceneMgr
{
	SET_SINGLETON(SceneMgr)

public:
	void Add(DX::Graphic* graphic, Scene* scene);
	void Remove(DX::Graphic* graphic, std::string key);
	void Update(DX::Graphic* graphic);
	void Render(DX::Graphic* graphic);

private:
	void Init();

	std::unordered_map<int, std::unordered_map<std::string, Scene*>> m_scenes;
};

