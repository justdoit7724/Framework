#pragma once
#include "Scene.h"
#include <vector>

class Object;
class Timer;
class DirectionalLight;
class PointLight;
class SpotLight;
class UICanvas;
class IGraphic;

class TestScene :
	public Scene
{
public:
	TestScene(IGraphic* graphic);
	~TestScene();
	void Update(Camera* camera)override;
	void Render()const override;

private:
	Camera* camera;

	UICanvas* canvas;

	std::vector<Object*> objs;

	DirectionalLight* dLight = nullptr;
	PointLight* pLight = nullptr;
	PointLight* pLight2 = nullptr;
};

