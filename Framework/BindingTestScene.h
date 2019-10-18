#pragma once
#include "Scene.h"
#include "Object.h"
class BindingTestScene :
	public Scene
{
public:
	BindingTestScene();

	void Logic_Update() override;
	void Render_Update(const Camera* camera, float elapsed, float spf) override;
	void Render()const override;

private:
	Object* obj, *obj2;
	bool b1 = false;
	bool bb1 = false;
};

