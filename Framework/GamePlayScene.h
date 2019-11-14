#pragma once
#include "Scene.h"

class TokenMgr;

class GamePlayScene : public Scene
{
public:
	GamePlayScene();
	~GamePlayScene();

	void Update(float elapsed, float spf)override;
	void Render(const Camera* camera, UINT sceneDepth)const override;

	void Message(UINT msg)override;

private:
	bool CameraFrameLerping(float spf);
	float curTime = 0;
	float camFrameLerpingTime = 5;
	XMFLOAT4X4 orthogonalP;
	XMFLOAT4X4 perspectiveP;
	XMMATRIX curP;
	bool CameraSliding(float spf);

	enum GAMEPLAY_STAGE
	{
		GAMEPLAY_STAGE_LOBBY,
		GAMEPLAY_STAGE_CAM_MODIFY,
		GAMEPLAY_STAGE_PLAY
	};
	GAMEPLAY_STAGE curStage;

	Camera* camera;

	TokenMgr* tokenMgr;
};
