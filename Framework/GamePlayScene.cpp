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
#include "Skybox.h"
#include "Debugging.h"
#include "TextureMgr.h"
#include "Quad.h"
#include "BlendState.h"

GamePlayScene::GamePlayScene()
{
	curStage = GAMEPLAY_STAGE_LOBBY;

	dLight = new DirectionalLight(
		XMFLOAT3(0.5f, 0.5f, 0.5f),
		XMFLOAT3(0.8f, 0.8f, 0.8f),
		XMFLOAT3(0.8f, 0.8f, 0.8f),
		Normalize(XMFLOAT3(2,-1,0)));

	camera = new Camera("GamePlay", FRAME_KIND_ORTHOGONAL, SCREEN_WIDTH/10, SCREEN_HEIGHT/10, 0.1f, 200.0f, NULL, NULL);
	camera->transform->SetTranslation(XMFLOAT3(0, 40, 0));
	camera->transform->SetRot(-UP, FORWARD);
	XMStoreFloat4x4(&orthogonalP, camera->StdProjMat());

	camera->SetFrame(FRAME_KIND_PERSPECTIVE, XMFLOAT2(NULL, NULL), 0.1f, 300.0f, XM_PIDIV2, 1);
	XMStoreFloat4x4(&perspectiveP, camera->StdProjMat());

	CameraMgr::Instance()->SetMain("GamePlay");

	nonaga = new NonagaStage();
	std::vector<Object*> gameObjs;
	nonaga->Objs(gameObjs);
	parentObj = new Object(new Quad(), nullptr, nullptr);
	parentObj->SetShow(false);
	for (auto go : gameObjs)
	{
		parentObj->AddChildren(go);
	}
	AddObj(parentObj);

	cbEye = new Buffer(sizeof(XMFLOAT4));

	Debugging::Instance()->EnableGrid(10);

	curP = XMMatrixIdentity();

	slideStartPt = camera->transform->GetPos();
	slideEndForward = Normalize(XMFLOAT3(0, -1, 4));
	slideEndPt = -slideEndForward * radFromCenter;
	slideEndUp = Normalize(XMFLOAT3(0, 4, 1));

	std::vector<std::string> cm;
	cm.push_back("Data\\Texture\\cm_px.jpg");
	cm.push_back("Data\\Texture\\cm_nx.jpg");
	cm.push_back("Data\\Texture\\cm_py.jpg");
	cm.push_back("Data\\Texture\\cm_ny.jpg");
	cm.push_back("Data\\Texture\\cm_pz.jpg");
	cm.push_back("Data\\Texture\\cm_nz.jpg");
	TextureMgr::Instance()->LoadCM("cm", cm);
	skybox = new Skybox(TextureMgr::Instance()->Get("cm"));
	AddObj(skybox);
	shadowMapping = new ShadowMap(4096, 4096, 256, 256);
	//ssao = new SSAOMap();
}

GamePlayScene::~GamePlayScene()
{
	delete cbEye;
	delete dLight;
	delete camera;
	delete nonaga;
}

void GamePlayScene::BindEye()
{
	XMFLOAT4 camEye = XMFLOAT4(
		CameraMgr::Instance()->Main()->transform->GetPos().x,
		CameraMgr::Instance()->Main()->transform->GetPos().y,
		CameraMgr::Instance()->Main()->transform->GetPos().z, 0);
	cbEye->Write(&camEye);
	DX_DContext->PSSetConstantBuffers(SHADER_REG_PS_CB_EYE, 1, cbEye->GetAddress());
}

void GamePlayScene::ObjMove(float spf)
{
	static float angleX = 0;
	static float angleY = 0;
	static XMFLOAT2 prevMousePt=XMFLOAT2(0,0);
	XMFLOAT3 cameraForward = camera->transform->GetForward();
	XMFLOAT2 mPt = Mouse::Instance()->Pos();
	if (Mouse::Instance()->RightState() == MOUSE_STATE_PRESSING)
	{
		angleX += angleSpeed * spf * (prevMousePt.x-mPt.x);

		angleY += (prevMousePt.y - mPt.y)*angleSpeed * spf;
		const float offset = acosf(Dot(cameraForward, FORWARD));
		angleY = Clamp(-XM_PIDIV2+offset, XM_PIDIV4+offset, angleY);
	}
	prevMousePt.x = mPt.x;
	prevMousePt.y = mPt.y;
	XMFLOAT3 u = MultiplyDir(UP, XMMatrixRotationX(angleY));
	XMFLOAT3 f = RotateFromDir(Cross(u, -RIGHT), u, angleX);
	parentObj->transform->SetRot(f, u);
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
		Debugging::Instance()->Draw("frame t = ", t, 10, 30);
		CameraFrameLerping(t);
		CameraSliding(Clamp(0,1,t-0.9));
		LightRotating(t);
		if (t>2)
			curStage = GAMEPLAY_STAGE_PLAY;
	}
		break;
	case GAMEPLAY_STAGE_PLAY:
		ObjMove(spf);
		Geometrics::Ray camRay;
		camera->Pick(&camRay);

		nonaga->Update(camRay);
		break;
	}


	camera->Update();

	BindEye();

	skybox->Mapping();
	shadowMapping->Mapping(this, dLight);
	//ssao->Mapping(this, camera);
}

void GamePlayScene::Render(const XMMATRIX& vp, const Frustum& frustum, UINT sceneDepth) const
{
	dLight->Apply();

	XMMATRIX curTempVP = vp;
	switch (curStage)
	{
	case GAMEPLAY_STAGE_LOBBY:
		curTempVP = camera->VMat()*XMLoadFloat4x4(&orthogonalP);
		break;
	case GAMEPLAY_STAGE_CAM_MODIFY:
		curTempVP = camera->VMat()*curP;
		break;
	case GAMEPLAY_STAGE_PLAY:
		break;
	}

	Scene::Render(curTempVP, frustum, sceneDepth);
	nonaga->Render(curTempVP, sceneDepth);

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
	float mt = sinf((t - 0.5f) * XM_PI + 1) / 2;

	XMFLOAT3 sPos = Lerp(slideStartPt, slideEndPt, mt);
	XMFLOAT3 sForward = Lerp(-UP, slideEndForward, mt);
	XMFLOAT3 sUp = Lerp(FORWARD, slideEndUp, mt);

	camera->transform->SetTranslation(-sForward* radFromCenter);
	camera->transform->SetRot(sForward, sUp);
}

void GamePlayScene::LightRotating(float t)
{

}
