#pragma once

#include "Singleton.h"

#define MSG_SCENE_AI_NORMAL_INTRO_START 0
#define MSG_SCENE_AI__HARD_INTRO_START 1
#define MSG_SCENE_PVP_INTRO_START 2

class Scene;

class SceneMgr :
	public Singleton<SceneMgr>
{
public:
	SceneMgr();
	~SceneMgr();
	void BroadcastMessage(unsigned int msg);
	const Scene* Get(std::string key){return list[key];}
	
	void Process(float wElapsed, float wSpf);
	void Add(std::string key, Scene* scene);
private:

	std::unordered_map<std::string,Scene*> list;
	std::vector<std::string> executeOrder;
};

