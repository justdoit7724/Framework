#pragma once
#include "Singleton.h"
#include <unordered_map>
#include <assert.h>

class Scene;

class SceneMgr :
	public Singleton<SceneMgr>
{
public:
	friend class Scene;

	void Add(Scene* scene);
	void Remove(std::string key)
	{
		list.erase(key);
	}
	Scene* operator[](std::string key)
	{
		return list[key];
	}

	void Process(float wElapsed, float wSpf);

private:
	std::unordered_map<std::string, Scene*> list;
};

