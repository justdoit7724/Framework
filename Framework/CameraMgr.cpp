#include "stdafx.h"
#include "CameraMgr.h"

void CameraMgr::Add(std::string key, const Camera* camera)
{
	if (list.find(key) == list.end())
	{
		list.insert(std::pair<std::string, const Camera*>(key, camera));
	}
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

const Camera* CameraMgr::Main()
{
	assert(list.find(mainKey) != list.end());
	return list[mainKey];
}
