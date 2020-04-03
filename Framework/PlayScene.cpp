#include "stdafx.h"
#include "PlayScene.h"
#include "UI.h"
#include "Shader.h"
#include "ShaderReg.h"
#include "TextureMgr.h"
#include "Game_info.h"
#include "Light.h"
#include "Buffer.h"
#include "Tictactoe.h"
#include "Keyboard.h"
#include "Debugging.h"

#define NOTIFY_MOVE 0

PlayScene::PlayScene()
{
	rule = new Tictactoe();

	dLight = new DirectionalLight(
		XMFLOAT3(0.4, 0.4, 0.4),
		XMFLOAT3(0.7, 0.7, 0.7),
		XMFLOAT3(0.9, 0.9, 0.9),
		Normalize(XMFLOAT3(1, -0.5, 1)));

	cbEye = new Buffer(sizeof(XMFLOAT4));

	canvas = new UICanvas();

	TextureMgr::Instance()->Load("moveO", "Data\\Texture\\TicTacToe\\o.png");
	TextureMgr::Instance()->Load("moveX", "Data\\Texture\\TicTacToe\\x.png");
	TextureMgr::Instance()->Load("moveNull", "Data\\Texture\\white.png");

	const float thirdScnWidth = 500/3.0f;
	const float thirdScnHeight = 500 / 3.0f;

	for (int y = 0; y < 3; ++y)
	{
		for (int x = 0; x < 3; ++x)
		{
			UIButton* newBtn = new UIButton(XMFLOAT2(x* thirdScnWidth + x*10, y* thirdScnHeight+y*10), XMFLOAT2(thirdScnWidth, thirdScnHeight),
				TextureMgr::Instance()->Get("moveNull"),
				TextureMgr::Instance()->Get("moveNull"),
				TextureMgr::Instance()->Get("moveNull"));
			newBtn->AddObserver(this);
			board[y][x] = SpaceInfo(x, y, newBtn);
			newBtn->SetNotify(NOTIFY_MOVE, &board[y][x]);
		}
	}
}


void PlayScene::Update(float elapsed, float spf)
{
	for (int y = 0; y < 3; ++y)
	{
		for (int x = 0; x < 3; ++x)
		{
			board[y][x].btn->Update(canvas);
		}
	}

	bool isOWin = false;
	if (rule->IsFinish(&isOWin))
	{
		Debugging::Instance()->Draw(isOWin ? "O win" : "X win", 500, 10, Colors::Green);
	}
}

void PlayScene::Notify(int id, const void* data)
{

	SpaceInfo* info = (SpaceInfo*)data;

	if (rule->Kind(info->x, info->y) != SpaceKind::Null/* || !PlayScene::IsO()*/)
	{
		return;
	}


	switch (id)
	{
	case NOTIFY_MOVE:
	{
		rule->Move(info->x, info->y);

		switch (rule->Kind(info->x, info->y))
		{
		case SpaceKind::O:
			info->btn->SetSRV(TextureMgr::Instance()->Get("moveO"));
			info->btn->SetHoverSRV(TextureMgr::Instance()->Get("moveO"));
			info->btn->SetPressSRV(TextureMgr::Instance()->Get("moveO"));
			break;

		case SpaceKind::X:
			info->btn->SetSRV(TextureMgr::Instance()->Get("moveX"));
			info->btn->SetHoverSRV(TextureMgr::Instance()->Get("moveX"));
			info->btn->SetPressSRV(TextureMgr::Instance()->Get("moveX"));
			break;
		}
	}
	break;
	}
}
