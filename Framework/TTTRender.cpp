#include "stdafx.h"
#include "TTTRender.h"
#include "UI.h"
#include "TextureMgr.h"
#include "Tictactoe.h"

TTTRender::TTTRender()
{
	canvas = new UICanvas();

	TextureMgr::Instance()->Load("moveO", "Data\\Texture\\TicTacToe\\o.png");
	TextureMgr::Instance()->Load("moveX", "Data\\Texture\\TicTacToe\\x.png");
	TextureMgr::Instance()->Load("moveNull", "Data\\Texture\\white.png");

	const float thirdScnWidth = 500 / 3.0f;
	const float thirdScnHeight = 500 / 3.0f;
	for (int y = 0; y < 3; ++y)
	{
		for (int x = 0; x < 3; ++x)
		{
			board[y][x] = new UIButton(XMFLOAT2(x * thirdScnWidth + x * 10, y * thirdScnHeight + y * 10), XMFLOAT2(thirdScnWidth, thirdScnHeight),
				TextureMgr::Instance()->Get("moveNull"),
				TextureMgr::Instance()->Get("moveNull"),
				TextureMgr::Instance()->Get("moveNull"));
			XMINT2* pIdx = new XMINT2(x, y);
			board[y][x]->SetNotify(1, pIdx);
		}
	}
}

void TTTRender::Update()
{
	for (int y = 0; y < 3; ++y)
	{
		for (int x = 0; x < 3; ++x)
		{
			board[y][x]->Update(canvas);
		}
	}
}

void TTTRender::MoveRender(int x, int y, SpaceKind kind)
{
	ID3D11ShaderResourceView* tempSRV = nullptr;
	switch (kind)
	{
	case Null:
		tempSRV = TextureMgr::Instance()->Get("moveNull");
		break;
	case O:
		tempSRV = TextureMgr::Instance()->Get("moveO");
		break;
	case X:
		tempSRV = TextureMgr::Instance()->Get("moveX");
		break;
	} 

	board[y][x]->SetSRV(tempSRV);
	board[y][x]->SetHoverSRV(tempSRV);
	board[y][x]->SetPressSRV(tempSRV);
}

void TTTRender::SetClickFunc(Observer* obs)
{
	for (int y = 0; y < 3; ++y)
	{
		for (int x = 0; x < 3; ++x)
		{
			board[y][x]->AddObserver(obs);
		}
	}
}
