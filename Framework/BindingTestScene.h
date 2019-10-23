#pragma once
#include "Scene.h"
#include "Object.h"
class BindingTestScene :
	public Scene
{
public:
	BindingTestScene();

	void Update(float elapsed, float spf) override;
	void Render(const Camera* camera, UINT sceneDepth)const override;

private:
	Object* obj, *obj2;
	bool b1 = false;
	bool bb1 = false;
};

