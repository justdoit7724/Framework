#include "GamePlayScene.h"
#include "Camera.h"
#include "Game_info.h"
#include "Transform.h"
#include "TokenMgr.h"
#include "CameraMgr.h"
#include "Debugging.h"
#include "SceneMgr.h"
#include "Mouse.h"
#include "Keyboard.h"

GamePlayScene::GamePlayScene()
{
	curStage = GAMEPLAY_STAGE_LOBBY;

	camera = new Camera("Lobby", FRAME_KIND_ORTHOGONAL, SCREEN_WIDTH, SCREEN_HEIGHT, 0.1f, 200.0f, NULL, NULL);
	camera->transform->SetTranslation(XMFLOAT3(0, 130, 0));
	camera->transform->SetRot(-UP, FORWARD);
	XMStoreFloat4x4(&orthogonalP, camera->ProjMat(Z_ORDER_STANDARD));

	camera->SetFrame(FRAME_KIND_PERSPECTIVE, XMFLOAT2(NULL, NULL), 0.1f, 300.0f, XM_PIDIV2, 1);
	XMStoreFloat4x4(&perspectiveP, camera->ProjMat(Z_ORDER_STANDARD));

	CameraMgr::Instance()->SetMain("Lobby");

	tokenMgr = new TokenMgr();

	Debugging::Instance()->EnableGrid(10);

	curP = XMMatrixIdentity();
}

GamePlayScene::~GamePlayScene()
{
	delete tokenMgr;
	delete camera;
}

void CameraMove(Camera* cam, float spf)
{
	XMFLOAT3 newPos = cam->transform->GetPos();
	XMFLOAT3 right = cam->transform->GetRight();
	XMFLOAT3 forward = cam->transform->GetForward();
	const float speed = 50;
	if (Keyboard::IsPressing('A')) {

		newPos += -right * speed * spf;
	}
	else if (Keyboard::IsPressing('D')) {

		newPos += right * speed * spf;
	}
	if (Keyboard::IsPressing('S')) {

		newPos += -forward * speed * spf;
	}
	else if (Keyboard::IsPressing('W')) {

		newPos += forward * speed * spf;
	}
	static float angleX = 0;
	static float angleY = 0;
	static XMFLOAT2 prevMousePt;
	const float angleSpeed = 3.141592f * 0.2f;
	XMFLOAT2 mPt = Mouse::Instance()->Pos();
	if (Mouse::Instance()->RightState() == MOUSE_STATE_PRESSING)
	{
		angleY += angleSpeed * spf * (mPt.x - prevMousePt.x);
		angleX += angleSpeed * spf * (mPt.y - prevMousePt.y);
	}
	prevMousePt.x = mPt.x;
	prevMousePt.y = mPt.y;
	const XMMATRIX rotMat = XMMatrixRotationX(angleX) * XMMatrixRotationY(angleY);
	cam->transform->SetTranslation(newPos);
	XMFLOAT3 f = MultiplyDir(FORWARD, rotMat);
	XMFLOAT3 u = MultiplyDir(UP, rotMat);
	cam->transform->SetRot(f, u);
	cam->Update();
}
void GamePlayScene::Update(float elapsed, float spf)
{
	camera->Update();

	switch (curStage)
	{
	case GAMEPLAY_STAGE_LOBBY:
		break;
	case GAMEPLAY_STAGE_CAM_MODIFY:
		if (CameraFrameLerping(spf) && CameraSliding(spf))
			curStage = GAMEPLAY_STAGE_PLAY;
		break;
	case GAMEPLAY_STAGE_PLAY:
		CameraMove(camera, spf);
		break;
	}

	Scene::Update(elapsed, spf);
}

void GamePlayScene::Render(const Camera* camera, UINT sceneDepth) const
{
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

	tokenMgr->Render(curTempVP, camera->transform->GetPos(), sceneDepth);

	Scene::Render(camera, sceneDepth);
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

bool GamePlayScene::CameraFrameLerping(float spf)
{
	curTime += spf;
	float t = pow(std::fminf(1,curTime / camFrameLerpingTime),16);
	Debugging::Instance()->Draw("t = ", t, 10, 10);

	curP = XMMATRIX(
		Lerp(orthogonalP._11, perspectiveP._11, t), 0, 0, 0,
		0, Lerp(orthogonalP._22, perspectiveP._22, t), 0, 0,
		0, 0, Lerp(orthogonalP._33, perspectiveP._33, t), Lerp(0, 1, t),
		0, 0, Lerp(orthogonalP._43, perspectiveP._43, t), Lerp(1, 0, t)
	);

	return (t>0.9f);
}

bool GamePlayScene::CameraSliding(float spf)
{
	return true;
}
