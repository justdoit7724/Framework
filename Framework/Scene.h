#pragma once

#include "DX_info.h"
#include <map>
#include <queue>

class Object;
class SceneMgr;

class Scene
{
public:

	virtual void Update(float elapsed, float spf)=0;

	virtual void Message(UINT msg) {};

	const std::string key;
protected:

private:
};

