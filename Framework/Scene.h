#pragma once
#include <vector>
#include "DX_info.h"


class Camera;
class Object;
class Timer;
class DirectionalLight;
class PointLight;
class SpotLight;
class UICanvas;

class Scene
{
public:
	Scene();
	~Scene();

	void Update();
	void Render();

private:

	Camera* camera;

	UICanvas* canvas;

	std::vector<Object*> objs;

	DirectionalLight* dLight;
};

