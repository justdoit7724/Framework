#pragma once
#include "Scene.h"

class SetupScene : public Scene
{
public:
	SetupScene();

	void Update(float elapsed, float spf) override;
};
