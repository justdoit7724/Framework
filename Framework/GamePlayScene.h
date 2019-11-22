#pragma once
#include "Scene.h"

class NonagaStage;
class DirectionalLight;
class Buffer;
class ShadowMap;
class SSAOMap;

class GamePlayScene : public Scene
{
public:
	GamePlayScene();
	~GamePlayScene();

	void Update(float elapsed, float spf)override;
	void Render(const Camera* camera, UINT sceneDepth)const override;

	void Message(UINT msg)override;

private:
	void BindEye();
	void CameraMove(Camera* cam, float spf);
	void CameraFrameLerping(float t);
	void CameraSliding(float t);
	void LightRotating(float t);
	float curTime = 0;
	const float camFrameLerpingTime = 1;

	XMFLOAT4X4 orthogonalP;
	XMFLOAT4X4 perspectiveP;
	XMMATRIX curP;

	enum GAMEPLAY_STAGE
	{
		GAMEPLAY_STAGE_LOBBY,
		GAMEPLAY_STAGE_CAM_MODIFY,
		GAMEPLAY_STAGE_PLAY
	};
	GAMEPLAY_STAGE curStage;

	DirectionalLight* dLight;

	Camera* camera;

	NonagaStage* nonaga;

	Buffer* cbEye;

	XMFLOAT3 slideStartPt, slideEndPt;
	XMFLOAT3 slideEndForward;
	XMFLOAT3 slideEndUp;

	const float radFromCenter = 60.0f;
	const float angleSpeed = 3.141592f * 0.2f;

	ShadowMap* shadowMapping;
	SSAOMap* ssao;
};

