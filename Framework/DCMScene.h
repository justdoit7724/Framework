#pragma once
#include "Scene.h"

class DynamicCubeMap;
class IGraphic;
class Timer;
class Mirror;

class DCMScene :
	public Scene
{
public:
	DCMScene(IGraphic* graphic, Scene* captureScene);
	~DCMScene();

	void Logic_Update(Camera* camera) override;
	void Render_Update(Camera* camera) override;
	void Render()const override;

private:
	Timer* timer;
	DynamicCubeMap* dcmObj;
	Mirror* mirror;
};

