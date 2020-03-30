#pragma once
#include "Scene.h"

class Buffer;
class Object;
class DirectionalLight;
class UICanvas;

class TestScene : public Scene
{
public:
	TestScene();
	~TestScene();

	void Update(float elapsed, float spf)override;
private:
	Buffer* cbEye;
	DirectionalLight* dLight;
	UICanvas* canvas;
};