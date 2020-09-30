#pragma once
#include "Scene.h"
class StartScene :
	public Scene
{
public:
	StartScene();
	~StartScene();

	void Update(float elapsed, float spf) override;
};

