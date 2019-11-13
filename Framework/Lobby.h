#pragma once
#include "Scene.h"

class UICanvas;

class Lobby : public Scene
{
public:
	Lobby();

	void Update(float elapsed, float spf) override;
	void Render(const Camera* camera, UINT sceneDepth)const override;

private:

	UICanvas* canvas;
};