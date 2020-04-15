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

PlayScene::PlayScene()
{
	Pathfinding::AStar* astar = new Pathfinding::AStar(XMFLOAT3(0,0,0),10,100,100);
	auto paths = astar->GetPaths();
	astar->FindPath(&paths[0], nullptr);

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
