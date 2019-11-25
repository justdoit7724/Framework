#include "Lobby.h"
#include "UI.h"
#include "Camera.h"
#include "TextureMgr.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "CameraMgr.h"
#include "Transform.h"
#include "SceneMgr.h"

#include "Debugging.h"

#define OBS_DATA_AI 0
#define OBS_DATA_PVP 1
#define OBS_DATA_AI_NORMAL 2
#define OBS_DATA_AI_HARD 3

Lobby::Lobby()
{
	curStage = LOBBY_STAGE_SELECT;

	TextureMgr::Instance()->Load("title", "Data\\Texture\\title.png");
	TextureMgr::Instance()->Load("aiButton", "Data\\Texture\\ai_button.png");
	TextureMgr::Instance()->Load("aiButtonHover", "Data\\Texture\\ai_button_hover.png");
	TextureMgr::Instance()->Load("aiButtonPress", "Data\\Texture\\ai_button_press.png");
	TextureMgr::Instance()->Load("pvpButton", "Data\\Texture\\pvp_button.png");
	TextureMgr::Instance()->Load("pvpButtonHover", "Data\\Texture\\pvp_button_hover.png");
	TextureMgr::Instance()->Load("pvpButtonPress", "Data\\Texture\\pvp_button_press.png");
	TextureMgr::Instance()->Load("diffNormal", "Data\\Texture\\difficultyNormal.png");
	TextureMgr::Instance()->Load("diffNormalHover", "Data\\Texture\\difficultyNormal_hover.png");
	TextureMgr::Instance()->Load("diffNormalPress", "Data\\Texture\\difficultyNormal_press.png");
	TextureMgr::Instance()->Load("diffHard", "Data\\Texture\\difficultyHard.png");
	TextureMgr::Instance()->Load("diffHardHover", "Data\\Texture\\difficultyHard_hover.png");
	TextureMgr::Instance()->Load("diffHardPress", "Data\\Texture\\difficultyHard_press.png");
	ID3D11ShaderResourceView* titleSRV = TextureMgr::Instance()->Get("title");
	ID3D11ShaderResourceView* aiSRV = TextureMgr::Instance()->Get("aiButton");
	ID3D11ShaderResourceView* aiHoverSRV = TextureMgr::Instance()->Get("aiButtonHover");
	ID3D11ShaderResourceView* aiPressSRV = TextureMgr::Instance()->Get("aiButtonPress");
	ID3D11ShaderResourceView* pvpSRV = TextureMgr::Instance()->Get("pvpButton");
	ID3D11ShaderResourceView* pvpHoverSRV = TextureMgr::Instance()->Get("pvpButtonHover");
	ID3D11ShaderResourceView* pvpPressSRV = TextureMgr::Instance()->Get("pvpButtonPress");
	ID3D11ShaderResourceView* diffNormalSRV = TextureMgr::Instance()->Get("diffNormal");
	ID3D11ShaderResourceView* diffNormalHoverSRV = TextureMgr::Instance()->Get("diffNormalHover");
	ID3D11ShaderResourceView* diffNormalPressSRV = TextureMgr::Instance()->Get("diffNormalPress");
	ID3D11ShaderResourceView* diffHardSRV = TextureMgr::Instance()->Get("diffHard");
	ID3D11ShaderResourceView* diffHardHoverSRV = TextureMgr::Instance()->Get("diffHardHover");
	ID3D11ShaderResourceView* diffHardPressSRV = TextureMgr::Instance()->Get("diffHardPress");

	canvas = new UICanvas(SCREEN_WIDTH, SCREEN_HEIGHT);
	XMFLOAT2 titleSize = XMFLOAT2(600, 180);
	XMFLOAT2 selectButtonSize = XMFLOAT2(180, 120);
	XMFLOAT2 diffButtonSize = XMFLOAT2(220, 120);
	title = new UI(canvas,XMFLOAT2(380 - titleSize.x * 0.5f, 170-titleSize.y*0.5f), titleSize, 0, titleSRV);
	singleButton = new UIButton(canvas, OBS_DATA_AI, nullptr,XMFLOAT2(380 - selectButtonSize.x * 0.5f, 370 - selectButtonSize.y * 0.5f), selectButtonSize,
		aiSRV, aiHoverSRV, aiPressSRV);
	pvpButton = new UIButton(canvas, OBS_DATA_PVP, nullptr, XMFLOAT2(380 - selectButtonSize.x * 0.5f, 520 - selectButtonSize.y * 0.5f), selectButtonSize,
		pvpSRV, pvpHoverSRV, pvpPressSRV);
	aiNormalButton = new UIButton(canvas, OBS_DATA_AI_NORMAL, nullptr, XMFLOAT2(380 - diffButtonSize.x * 0.5f, 370 - diffButtonSize.y * 0.5f), diffButtonSize,
		diffNormalSRV, diffNormalHoverSRV, diffNormalPressSRV);
	aiHardButton = new UIButton(canvas, OBS_DATA_AI_HARD, nullptr, XMFLOAT2(380 - diffButtonSize.x * 0.5f, 520 - diffButtonSize.y * 0.5f), diffButtonSize,
		diffHardSRV, diffHardHoverSRV, diffHardPressSRV);
	singleButton->AddObserver(this);
	pvpButton->AddObserver(this);
	aiNormalButton->SetEnabled(false);
	aiHardButton->SetEnabled(false);
}
Lobby::~Lobby()
{
	delete canvas;
	delete title;
	delete singleButton;
	delete pvpButton;
	delete aiNormalButton;
	delete aiHardButton;
}
void Lobby::Update(float elapsed, float spf)
{
	const float fadeDuration = 2;
	float fadeT = -spf / fadeDuration;
	Debugging::Instance()->Draw("Fade t = ", fadeT, 10, 10);

	switch (curStage)
	{
	case Lobby::LOBBY_STAGE_SELECT:
		canvas->Update(spf);
		break;
	case Lobby::LOBBY_STAGE_FADEOUT:
		title->Fade(fadeT);
		singleButton->Fade(fadeT);
		pvpButton->Fade(fadeT);
		aiNormalButton->Fade(fadeT);
		aiHardButton->Fade(fadeT);
		if (aiNormalButton->GetTransp() <= 0)
			SceneMgr::Instance()->SafeDeleteScene("Lobby");
		break;
	}
}

void Lobby::Render(const XMMATRIX& vp, const Frustum& frustum, UINT sceneDepth) const
{
	canvas->Render(sceneDepth);
}	

void Lobby::Notify(unsigned int id, const void* data)
{
	switch (id)
	{
	case OBS_DATA_AI:
		singleButton->SetEnabled(false);
		pvpButton->SetEnabled(false);
		aiNormalButton->SetEnabled(true);
		aiHardButton->SetEnabled(true);
		break;
	case OBS_DATA_PVP:
		SceneMgr::Instance()->BroadcastMessage(MSG_SCENE_PVP_INTRO_START);
		curStage = LOBBY_STAGE_FADEOUT;
		break;
	case OBS_DATA_AI_NORMAL:
		SceneMgr::Instance()->BroadcastMessage(MSG_SCENE_AI_NORMAL_INTRO_START);
		curStage = LOBBY_STAGE_FADEOUT;
		break;
	case OBS_DATA_AI_HARD:
		SceneMgr::Instance()->BroadcastMessage(MSG_SCENE_AI__HARD_INTRO_START);
		curStage = LOBBY_STAGE_FADEOUT;
		break;
	}
}
