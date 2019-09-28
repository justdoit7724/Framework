#pragma once
#include "Singleton.h"
#include <unordered_map>
#include <assert.h>

class Scene;

class SceneMgr :
	public Singleton<SceneMgr>
{
public:
	void Add(std::string key, Scene* scene)
	{
		assert(list.find(key) == list.end());

		list.insert(std::pair<std::string, Scene *>(key, scene));
	}
	void Remove(std::string key)
	{
		list.erase(key);
	}
	Scene* operator[](std::string key)
	{
		return list[key];
	}

	void Process();

private:
	std::unordered_map<std::string, Scene*> list;
};

