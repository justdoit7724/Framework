#include "stdafx.h"
#include "PlayScene.h"
#include "Shader.h"
#include "ShaderReg.h"
#include "Game_info.h"
#include "Light.h"
#include "Buffer.h"
#include "Tictactoe.h"
#include "Keyboard.h"
#include "TTTAI.h"
#include "TTTRender.h"
#include "Debugging.h"


PlayScene::PlayScene()
{

	render = new TTTRender();
	rule = new Tictactoe(&render);
	ai = new TTTAI(&rule, &render);
	render->SetClickFunc(rule);

	dLight = new DirectionalLight(
		XMFLOAT3(0.4, 0.4, 0.4),
		XMFLOAT3(0.7, 0.7, 0.7),
		XMFLOAT3(0.9, 0.9, 0.9),
		Normalize(XMFLOAT3(1, -0.5, 1)));

	cbEye = new Buffer(sizeof(XMFLOAT4));

	Debugging::Instance()->Enabled(false);
}


void PlayScene::Update(float elapsed, float spf)
{
	render->Update();

	if (Keyboard::GetKey('A') == KeyState_Up)
	{
		rule->JumpBackward();
	}
	else if (Keyboard::GetKey('D') == KeyState_Up)
	{
		rule->JumpForward();
	}
}
