#pragma once
#include "DX_info.h"
#include <vector>

class Camera;
class Object;
class VShader;

class Scene
{
public:
	Scene(std::string key);
	~Scene();

	virtual void Update_Logic() {};
	virtual void Render_Update(const Camera* camera, float elapsed, float spf);
	virtual void Render()const;
	virtual void ShadowCapture(const Camera* camera) {}
	void FrustumCulling(const Camera* camera);

	const std::string key;
protected:
	void AddObj(Object* obj) { objs.push_back(obj); }
private:
	std::vector<Object*> objs;
	std::vector<Object*> drawObjs;
};

