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
class IGraphic;

class Scene
{
public:
	Scene(IGraphic* graphic);
	~Scene();

	void Update();
	void Render();

	IGraphic *const graphic;

private:

	Camera* camera;

	UICanvas* canvas;

	std::vector<Object*> objs;

	DirectionalLight* dLight = nullptr;
	PointLight* pLight=nullptr;
	PointLight* pLight2=nullptr;
};

