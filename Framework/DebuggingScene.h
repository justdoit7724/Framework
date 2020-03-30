#pragma once
#include "Scene.h"
class DebuggingScene :
	public Scene
{
public:
	DebuggingScene();
	~DebuggingScene();

	void Update(float elapsed, float spf)override;
};