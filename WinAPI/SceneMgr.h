#pragma once

#include "Singleton.h"

class Scene;

class SceneMgr :
	public Singleton<SceneMgr>
{
public:
	SceneMgr();
	~SceneMgr();
	void BroadcastMessage(unsigned int msg);

	void Process(float wElapsed, float wSpf);
	void Add(const wchar_t* strID, Scene* scene);
private:

	std::unordered_map<const wchar_t*, Scene*> list;
	std::vector<const wchar_t*> executeOrder;

};


