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
	void Logic_Update()override;
	void Render_Update(const Camera* camera, float elapsed)override;
	void Render()const override;
	void ShadowCapture(const Camera* camera) override;

private:

	IGraphic* graphic;
	Timer* timer;

	UICanvas* canvas;

	DirectionalLight* dLight = nullptr;
	PointLight* pLight = nullptr;
	PointLight* pLight2 = nullptr;
};

