#pragma once
#include "Scene.h"
#include "ObserverDP.h"

class UICanvas;
class UI;
class UIButton;
class Camera;

class Lobby : public Scene, public Observer
{
public:
	Lobby();
	~Lobby();

	void Update(float elapsed, float spf) override;
	void Render(const Camera* camera, UINT sceneDepth)const override;

	void Notify(const void* data)override;

private:

	enum LOBBY_STAGE
	{
		LOBBY_STAGE_SELECT,
		LOBBY_STAGE_FADEOUT
	}curStage;
	

	UICanvas* canvas;
	UI* title;
	UIButton* singleButton;
	UIButton* pvpButton;
	UIButton* aiNormalButton;
	UIButton* aiHardButton;

};