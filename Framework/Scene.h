#pragma once
#include <vector>
#include "DX_info.h"


class Camera;
class Object;
class Timer;
class IGraphic;
class DirectionalLight;
class PointLight;
class SpotLight;
class UICanvas;

class Scene
{
public:
	Scene(IGraphic* graphic);
	~Scene();

	void Update();
	void Render(IGraphic* graphic);

private:

	Camera* camera;

	UICanvas* canvas;

	std::vector<Object*> objs;

	DirectionalLight* dLight;
};

