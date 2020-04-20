#pragma once
#include "Scene.h"
#include "ObserverDP.h"

class DirectionalLight;
class Buffer;
class UICanvas;
class UI;

class PlayScene : public Scene, public Observer
{
public:
	PlayScene();
	void Update(float elapsed, float spf) override;

private:

	DirectionalLight* dLight;
	Buffer* cbEye;
	UICanvas* canvas;

	// Inherited via Observer
	virtual void Notify(int id, const void* data) override;
};
