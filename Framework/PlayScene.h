#pragma once
#include "Scene.h"
#include "ObserverDP.h"

class DirectionalLight;
class Buffer;
class UICanvas;
class UI;
class UIButton;

class PlayScene : public Scene, public Observer
{
public:
	PlayScene();
	void Update(float elapsed, float spf) override;

private:

	DirectionalLight* dLight;
	Buffer* cbEye;
	UICanvas* canvas;

	UI* uiBackground;
	UI* mask;
	UI* mainImage;
	UIButton* moveModeBtn;
	UIButton* shapeCircleModeBtn;
	UIButton* shapeSquareModeBtn;

	// Inherited via Observer
	virtual void Notify(int id, const void* data) override;
};
