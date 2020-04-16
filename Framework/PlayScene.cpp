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
#include "Timer.h"

UIButton** btns = nullptr;
const int pathX = 100;
const int pathZ = 100;
Pathfinding::AStar* astar;
Pathfinding::Path* startP=nullptr;
Pathfinding::Path* destP=nullptr;
CustomSTL::SLNode<const Pathfinding::Path*>* iWayNode = nullptr;
PlayScene::PlayScene()
{
	canvas = new UICanvas();

	btns = new UIButton*[pathX * pathZ];

	astar = new Pathfinding::AStar(XMFLOAT3(0,0,0),10,pathX,pathZ);
	auto paths = astar->GetPaths();
	
	TextureMgr::Instance()->Load("red", "Data\\Texture\\red_light.png");
	TextureMgr::Instance()->Load("green", "Data\\Texture\\green_light.png");
	TextureMgr::Instance()->Load("blue", "Data\\Texture\\blue_light.png");
	TextureMgr::Instance()->Load("start", "Data\\Texture\\sample3.jpg");
	TextureMgr::Instance()->Load("dest", "Data\\Texture\\test_normal.png");

	for (int z = 0; z < pathZ; ++z)
	{
		for (int x = 0; x < pathX; ++x)
		{
			btns[x+z*pathX] = new UIButton(XMFLOAT2(x * SCREEN_WIDTH / pathX, z* SCREEN_HEIGHT / pathZ), XMFLOAT2(45	,45),
				paths[x + z * pathX].isBlock ? TextureMgr::Instance()->Get("red") : TextureMgr::Instance()->Get("blue"),
				paths[x + z * pathX].isBlock ? TextureMgr::Instance()->Get("green") : TextureMgr::Instance()->Get("red"),
				paths[x + z * pathX].isBlock ? TextureMgr::Instance()->Get("red") : TextureMgr::Instance()->Get("blue"));
			btns[x + z * pathX]->SetNotify(0, &paths[x + z * pathX]);
			btns[x + z * pathX]->AddObserver(this);
		}
	}


	dLight = new DirectionalLight(
		XMFLOAT3(0.4, 0.4, 0.4),
		XMFLOAT3(0.7, 0.7, 0.7),
		XMFLOAT3(0.9, 0.9, 0.9),
		Normalize(XMFLOAT3(1, -0.5, 1)));

	cbEye = new Buffer(sizeof(XMFLOAT4));
}


void PlayScene::Update(float elapsed, float spf)
{
	if (Keyboard::GetKey('A') == KeyState_Down)
	{
		if (astar->FindPath(startP, destP, &iWayNode))
		{
			while (iWayNode)
			{
				btns[iWayNode->data->idx]->SetShow(false);
				iWayNode = iWayNode->next;
			}
		}
	}

	for (int z = 0; z < pathZ; ++z)
	{
		for (int x = 0; x < pathX; ++x)
		{
			btns[x + z * pathX]->Update(canvas);
		}
	}
}

void PlayScene::Notify(int id, const void* data)
{
	auto mData = (Pathfinding::Path*)data;

	switch (id)
	{
	case 0:
		if (!startP)
		{
			startP = mData;
			btns[startP->idx]->SetSRV(TextureMgr::Instance()->Get("start"));
			btns[startP->idx]->SetHoverSRV(TextureMgr::Instance()->Get("start"));
			btns[startP->idx]->SetPressSRV(TextureMgr::Instance()->Get("start"));
		}
		else if (!destP)
		{
			destP = mData;
			btns[destP->idx]->SetSRV(TextureMgr::Instance()->Get("dest"));
			btns[destP->idx]->SetHoverSRV(TextureMgr::Instance()->Get("dest"));
			btns[destP->idx]->SetPressSRV(TextureMgr::Instance()->Get("dest"));
		}
		else
		{
			mData->isBlock = !mData->isBlock;
			if (mData->isBlock)
			{
				btns[mData->idx]->SetSRV(TextureMgr::Instance()->Get("red"));
				btns[mData->idx]->SetHoverSRV(TextureMgr::Instance()->Get("red"));
				btns[mData->idx]->SetPressSRV(TextureMgr::Instance()->Get("red"));
			}
			else {
				btns[mData->idx]->SetSRV(TextureMgr::Instance()->Get("blue"));
				btns[mData->idx]->SetHoverSRV(TextureMgr::Instance()->Get("blue"));
				btns[mData->idx]->SetPressSRV(TextureMgr::Instance()->Get("blue"));
			}
		}
		break;
	}
}
