#pragma once
#include "Scene.h"
class DebuggingScene :
	public Scene
{
public:
	DebuggingScene();

	void Logic_Update()override;
	void Render_Update(const Camera* camera)override;
	void Render()const override;
};

