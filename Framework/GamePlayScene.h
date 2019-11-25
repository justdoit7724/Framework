#pragma once
#include "Scene.h"

class NonagaStage;
class DirectionalLight;
class Buffer;
class ShadowMap;
class Camera;
class SSAOMap;
class Skybox;

class GamePlayScene : public Scene
{
public:
	GamePlayScene();
	~GamePlayScene();

	void Update(float elapsed, float spf) override;
	void Render(const XMMATRIX& vp, const Frustum& frustum, UINT sceneDepth)const override;

	void Message(UINT msg)override;

private:
	void BindEye();
	void ObjMove(float spf);
	void CameraFrameLerping(float t);
	void CameraSliding(float t);
	void LightRotating(float t);
	float curTime = 0;
	const float camFrameLerpingTime = 2;

	XMFLOAT4X4 orthogonalP;
	XMFLOAT4X4 perspectiveP;
	XMMATRIX curP;

	enum GAMEPLAY_STAGE
	{
		GAMEPLAY_STAGE_LOBBY=0,
		GAMEPLAY_STAGE_CAM_MODIFY=1,
		GAMEPLAY_STAGE_PLAY=2
	};
	GAMEPLAY_STAGE curStage;

	DirectionalLight* dLight;

	Camera* camera;

	NonagaStage* nonaga; 
	Object* parentObj;

	Buffer* cbEye;

	XMFLOAT3 slideStartPt, slideEndPt;
	XMFLOAT3 slideEndForward;
	XMFLOAT3 slideEndUp;

	const float radFromCenter = 60.0f;
	const float angleSpeed = 3.141592f * 0.1f;

	Skybox* skybox;
	ShadowMap* shadowMapping;
	SSAOMap* ssao;
};

