#pragma once

#include "DX_info.h"
#include <map>
#include <queue>

class Object;
class SceneMgr;
struct Frustum;

class Scene
{
public:
	virtual ~Scene();

	virtual void Update(float elapsed, float spf);
	virtual void Render(const XMMATRIX& vp, const Frustum& frustum, UINT sceneDepth, void const* subject)const;
	void AddObj(Object* obj);

	virtual void Message(UINT msg) {};
	bool Enabled() { return enabled; }
	void SetEnabled(bool e) { enabled = e; }

	const std::string key;
	void GetObjs(std::vector<const Object*>& rObj)const;
protected:

private:
	bool enabled=false;
	std::vector<Object*> objs;
};

