#include "stdafx.h"
#include "CameraMgr.h"

void CameraMgr::Add(const Camera* camera)
{
	if (!mainCamera)
		mainCamera = camera;

	assert(data.find(camera) == data.end());
	
	data.insert(camera);
}

void CameraMgr::Remove(const Camera* camera)
{
	assert(data.find(camera) != data.end());
	data.erase(camera);
}
