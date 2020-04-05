#pragma once

#include "Singleton.h"
#include <Windows.h>

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
	void BroadcastMessage(UINT msg);
	const Scene* Get(std::string key){return list[key];}
	
private:
	friend int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

	void Process(float wElapsed, float wSpf);
	void Add(std::string key, Scene* scene);
	std::unordered_map<std::string,Scene*> list;
	std::vector<std::string> executeOrder;
};

