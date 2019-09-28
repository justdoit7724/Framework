#include "CameraMgr.h"
#include <assert.h>

void CameraMgr::Add(std::string key, Camera* camera)
{
	assert(list.find(key) == list.end());
	list.insert(std::pair<std::string, Camera*>(key, camera));
}

void CameraMgr::Remove(std::string key)
{
	assert(list.find(key) != list.end());
	list.erase(key);
}

void CameraMgr::SetMain(std::string key)
{
	mainKey = key;
}

Camera* CameraMgr::Main()
{
	assert(list.find(mainKey) != list.end());
	return list[mainKey];
}
