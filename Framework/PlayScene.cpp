#include "stdafx.h"
#include "PlayScene.h"
#include "Shader.h"
#include "ShaderReg.h"
#include "Game_info.h"
#include "Light.h"
#include "Buffer.h"
#include "Keyboard.h"
#include "Debugging.h"
#include "CustomSTL.h"
#include "AStar.h"
#include "ObjectPool.h"
#include "UI.h"
#include "TextureMgr.h"
#include "DepthStencilState.h"
#include "Timer.h"
#include "Camera.h"
#include "Debugging.h"

#include "WICTexture.h"

PlayScene::PlayScene()
{
	TextureMgr::Instance()->Load("red", "Data\\Texture\\red_light.png");
	TextureMgr::Instance()->Load("green", "Data\\Texture\\green_light.png");
	TextureMgr::Instance()->Load("blue", "Data\\Texture\\blue_light.png");
	TextureMgr::Instance()->Load("white", "Data\\Texture\\white.png");
	TextureMgr::Instance()->Load("sample", "Data\\Texture\\sample.jpg");

	canvas = new UICanvas();

	dLight = new DirectionalLight(
		XMFLOAT3(0.4, 0.4, 0.4),
		XMFLOAT3(0.7, 0.7, 0.7),
		XMFLOAT3(0.9, 0.9, 0.9),
		Normalize(XMFLOAT3(1, -0.5, 1)));

	cbEye = new Buffer(sizeof(XMFLOAT4));
}


void PlayScene::Update(float elapsed, float spf)
{
}

void PlayScene::Notify(int id, const void* data)
{
	
}
