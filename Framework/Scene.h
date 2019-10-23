#pragma once
#include "DX_info.h"
#include <vector>

class Camera;
class Object;

class Scene 
{
public:
	Scene(std::string key);
	~Scene();

	virtual void Update(float elapsed, float spf);
	virtual void Render(const Camera* camera, UINT sceneDepth)const;
	void FrustumCulling(const Camera* camera);

	const std::string key;
protected:
	void AddObj(Object* obj) { objs.push_back(obj); }
private:
	std::vector<Object*> objs;
	std::vector<Object*> drawObjs;
};

