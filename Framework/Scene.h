#pragma once
#include "DX_info.h"
#include <vector>

class Object;
class SceneMgr;
class Camera;

class Scene 
{
public:
	virtual void Update(float elapsed, float spf);
	virtual void Render(const Camera* camera, UINT sceneDepth)const;

	virtual void Message(UINT msg) {};

	const std::string key;
protected:
	void AddObj(Object* obj) { objs.push_back(obj); }

private:
	friend class SceneMgr;
	bool enabled=false;
	std::vector<Object*> objs;
};

