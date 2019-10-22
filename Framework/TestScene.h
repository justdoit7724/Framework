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
class Timer;

class TestScene :
	public Scene
{
public:
	TestScene(IGraphic* graphic);
	~TestScene();
	void Update_Logic()override;
	void Render_Update(const Camera* camera, float elapsed, float spf)override;
	void Render()const override;

private:

	IGraphic* graphic;
	Timer* timer;

	UICanvas* canvas;

	DirectionalLight* dLight = nullptr;
	PointLight* pLight = nullptr;
	PointLight* pLight2 = nullptr;
};

