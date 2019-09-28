#pragma once
#include "Scene.h"
class DebuggingScene :
	public Scene
{
public:
	DebuggingScene();

	void Logic_Update(Camera* camera)override;
	void Render_Update(Camera* camera)override;
	void Render()const override;
};

