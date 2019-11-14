#pragma once
#include "Singleton.h"
#include <vector>
#include <unordered_map>
#include <assert.h>
#include <Windows.h>
#include <string>

#define MSG_SCENE_AI_NORMAL_INTRO_START 0
#define MSG_SCENE_AI__HARD_INTRO_START 1
#define MSG_SCENE_PVP_INTRO_START 2

class Scene;

class SceneMgr :
	public Singleton<SceneMgr>
{
public:
	void BroadcastMessage(UINT msg);
	void SafeDeleteScene(std::string key);
	void SetEnabled(std::string key, bool e);
	const Scene* Get(std::string key){return list[key];}
	
private:
	friend int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

	void Process(float wElapsed, float wSpf);
	void Add(std::string key, Scene* scene);
	std::unordered_map<std::string,Scene*> list;
	std::vector<std::string> executeOrder;
	std::vector<std::string> deleteList;
};

