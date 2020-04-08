#pragma once
#include "Scene.h"

class DirectionalLight;
class Buffer;

class PlayScene : public Scene
{
public:
	PlayScene();
	void Update(float elapsed, float spf) override;

private:

	DirectionalLight* dLight;
	Buffer* cbEye;
};
