#include "GamePlayScene.h"
#include "Camera.h"
#include "Game_info.h"
#include "Transform.h"
#include "CameraMgr.h"
#include "Debugging.h"
#include "SceneMgr.h"
#include "Mouse.h"
#include "Buffer.h"
#include "Light.h"
#include "Nonaga.h"
#include "ShaderReg.h"
#include "ShadowMap.h"
#include "SSAOMapping.h"

GamePlayScene::GamePlayScene()
{
	curStage = GAMEPLAY_STAGE_LOBBY;

	dLight = new DirectionalLight(
		XMFLOAT3(0.5f, 0.5f, 0.5f),
		XMFLOAT3(0.8f, 0.8f, 0.8f),
		XMFLOAT3(0.8f, 0.8f, 0.8f),
		Normalize(XMFLOAT3(2,-1,0)));

	camera = new Camera("GamePlay", FRAME_KIND_ORTHOGONAL, SCREEN_WIDTH, SCREEN_HEIGHT, 0.1f, 200.0f, NULL, NULL);
	camera->transform->SetTranslation(XMFLOAT3(0, 40, 0));
	camera->transform->SetRot(-UP, FORWARD);
	XMStoreFloat4x4(&orthogonalP, camera->ProjMat(Z_ORDER_STANDARD));

	camera->SetFrame(FRAME_KIND_PERSPECTIVE, XMFLOAT2(NULL, NULL), 0.1f, 300.0f, XM_PIDIV2, 1);
	XMStoreFloat4x4(&perspectiveP, camera->ProjMat(Z_ORDER_STANDARD));

	CameraMgr::Instance()->SetMain("GamePlay");

	nonaga = new NonagaStage();
	std::vector<Object*> gameObjs;
	nonaga->Objs(gameObjs);
	for (auto go : gameObjs)
	{
		AddObj(go);
	}

	cbEye = new Buffer(sizeof(XMFLOAT4));

	Debugging::Instance()->EnableGrid(10);

	curP = XMMatrixIdentity();

	slideStartPt = camera->transform->GetPos();
	slideEndForward = Normalize(XMFLOAT3(0, -1, 4));
	slideEndPt = -slideEndForward * radFromCenter;
	slideEndUp = Normalize(XMFLOAT3(0, 4, 1));

	shadowMapping = new ShadowMap(4096, 4096, 256, 256);
	ssao = new SSAOMap();
}

GamePlayScene::~GamePlayScene()
{
	delete dLight;
	delete camera;
	delete nonaga;
	delete cbEye;
}

void GamePlayScene::BindEye()
{
	XMFLOAT4 camEye = XMFLOAT4(
		camera->transform->GetPos().x,
		camera->transform->GetPos().y,
		camera->transform->GetPos().z, 0);
	cbEye->Write(&camEye);
	DX_DContext->PSSetConstantBuffers(SHADER_REG_PS_CB_EYE, 1, cbEye->GetAddress());
}

void GamePlayScene::CameraMove(Camera* cam, float spf)
{
	static float angleX = 0;
	static float angleY = 0;
	static XMFLOAT2 prevMousePt;
	XMFLOAT2 mPt = Mouse::Instance()->Pos();
	if (Mouse::Instance()->RightState() == MOUSE_STATE_PRESSING)
	{
		angleY += angleSpeed * spf * (mPt.x - prevMousePt.x);
		angleX = Clamp(-XM_PIDIV2, XM_PIDIV4,angleX + angleSpeed * spf * (mPt.y - prevMousePt.y));
	}
	prevMousePt.x = mPt.x;
	prevMousePt.y = mPt.y;
	const XMMATRIX rotMat = XMMatrixRotationX(angleX) * XMMatrixRotationY(angleY);
	XMFLOAT3 f = MultiplyDir(slideEndForward, rotMat);
	XMFLOAT3 u = MultiplyDir(slideEndUp, rotMat);
	cam->transform->SetRot(f, u);
	cam->transform->SetTranslation(-f*radFromCenter);
	cam->Update();
}
void GamePlayScene::Update(float elapsed, float spf)
{
	Scene::Update(elapsed, spf);

	dLight->SetDir(MultiplyDir(dLight->GetDir(), XMMatrixRotationY(elapsed * 0.0002f)));

	switch (curStage)
	{
	case GAMEPLAY_STAGE_LOBBY:
		break;
	case GAMEPLAY_STAGE_CAM_MODIFY:
	{
		curTime += spf;

		float t = curTime / camFrameLerpingTime;
		CameraFrameLerping(t);
		CameraSliding(t);
		LightRotating(t);
		if (t>2)
			curStage = GAMEPLAY_STAGE_PLAY;
	}
		break;
	case GAMEPLAY_STAGE_PLAY:
		CameraMove(camera, spf);
		Geometrics::Ray camRay;
		camera->Pick(&camRay);

		nonaga->Update(camRay);
		break;
	}

	BindEye();
	shadowMapping->Mapping(this, dLight);
	ssao->Mapping(this, camera);
}

void GamePlayScene::Render(const Camera* camera, UINT sceneDepth) const
{
	dLight->Apply();

	XMMATRIX curTempVP = XMMatrixIdentity();
	switch (curStage)
	{
	case GAMEPLAY_STAGE_LOBBY:
		curTempVP = camera->VMat()*XMLoadFloat4x4(&orthogonalP);
		break;
	case GAMEPLAY_STAGE_CAM_MODIFY:
		curTempVP = camera->VMat()*curP;
		break;
	case GAMEPLAY_STAGE_PLAY:
		curTempVP = camera->VMat() * camera->ProjMat(Z_ORDER_STANDARD);
		break;
	}

	Scene::Render(camera, sceneDepth);
	nonaga->Render(curTempVP, camera->transform->GetPos(), sceneDepth);

}

void GamePlayScene::Message(UINT msg)
{
	switch (msg)
	{
	case MSG_SCENE_PVP_INTRO_START:
		curStage = GAMEPLAY_STAGE_CAM_MODIFY;
		break;
	case MSG_SCENE_AI_NORMAL_INTRO_START:
		curStage = GAMEPLAY_STAGE_CAM_MODIFY;
		break;
	case MSG_SCENE_AI__HARD_INTRO_START:
		curStage = GAMEPLAY_STAGE_CAM_MODIFY;
		break;
	}

}

void GamePlayScene::CameraFrameLerping(float t)
{
	curP = XMMATRIX(
		Lerp(orthogonalP._11, perspectiveP._11, t), 0, 0, 0,
		0, Lerp(orthogonalP._22, perspectiveP._22, t), 0, 0,
		0, 0, Lerp(orthogonalP._33, perspectiveP._33, t), Lerp(0, 1, t),
		0, 0, Lerp(orthogonalP._43, perspectiveP._43, t), Lerp(1, 0, t)
	);
}

void GamePlayScene::CameraSliding(float t)
{
	float mt = fmaxf(0,t - 0.9f);

	XMFLOAT3 sPos = Lerp(slideStartPt, slideEndPt, mt);
	XMFLOAT3 sForward = Lerp(-UP, slideEndForward, mt);
	XMFLOAT3 sUp = Lerp(FORWARD, slideEndUp, mt);

	camera->transform->SetTranslation(sPos);
	camera->transform->SetRot(sForward, sUp);
}

void GamePlayScene::LightRotating(float t)
{
}
