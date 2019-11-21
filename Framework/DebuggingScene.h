#pragma once
#include "Scene.h"
class DebuggingScene :
	public Scene
{
public:
	DebuggingScene();
	~DebuggingScene();

	void Update(float elapsed, float spf)override;
	void Render(const Camera* camera, UINT sceneDepth)const override;
};

