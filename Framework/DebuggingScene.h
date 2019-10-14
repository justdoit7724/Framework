#pragma once
#include "Scene.h"
class DebuggingScene :
	public Scene
{
public:
	DebuggingScene();

	void Logic_Update()override;
	void Render_Update(const Camera* camera, float elapsed, float spf)override;
	void Render()const override;
};

