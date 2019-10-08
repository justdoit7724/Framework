#pragma once
#include "Singleton.h"
#include <string>
#include <unordered_map>

class Camera;

class CameraMgr :
	public Singleton<CameraMgr>
{
public:
	void Add(std::string key, Camera* camera);
	void Remove(std::string key);
	void SetMain(std::string key);
	Camera* Main();

private:
	std::string mainKey="";
	std::unordered_map<std::string, Camera*> list;
};
