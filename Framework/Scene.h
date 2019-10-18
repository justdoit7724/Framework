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

	virtual void Logic_Update() {};
	virtual void Render_Update(const Camera* camera, float elapsed);
	virtual void Render()const;
	virtual void ShadowCapture(const Camera* camera) {}

	const std::string key;
protected:
	void AddObj(Object* obj) { objs.push_back(obj); }
	std::vector<Object*> objs;
	VShader* shadowMapVS;
private:
};

