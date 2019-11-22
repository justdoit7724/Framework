#include "Nonaga.h"
#include "Debugging.h"
#include "Mouse.h"
#include "Token.h"
#include "Tile.h"
#include "PlaySpace.h"
#include "Transform.h"
#include <vector>

/* 2d array tile.
Only even index tile will be used in order to implement hexagonal arrangement of tile.
Transformation needed after.
■□■□■□...
□■□■□■...
■□■□■□...
......
......
......
*/
//Either empty or occupied tile space
#define TILE_SPACE_COUNT_X 25
#define TILE_SPACE_COUNT_Z 25
#define TILE_SPACE_INTERVAL_X 10.0f
#define TILE_SPACE_INTERVAL_Z 12.0f

NonagaStage::NonagaStage()
{
	curPlayState = PLAY_STATE_P1_TOKEN;

	logic = new NonagaLogic(playSpace);

	// create tile space
	int centerIDZ = TILE_SPACE_COUNT_Z / 2.0f;
	int centerIDX = TILE_SPACE_COUNT_X / 2.0f;

	tileSpaceMat = XMMATRIX(
		TILE_SPACE_INTERVAL_X, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, TILE_SPACE_INTERVAL_Z / 2.0f, 0,
		-TILE_SPACE_INTERVAL_X * centerIDX, 0, -TILE_SPACE_INTERVAL_Z * centerIDZ / 2.0f, 1);

	invTileSpaceMat = XMMatrixInverse(&XMMatrixDeterminant(tileSpaceMat), tileSpaceMat);

	detectPlane = Geometrics::PlaneInf(XMFLOAT3(0, 0, 0), UP);

	int tileObjIdx = 0;

	for (int z = 0; z < TILE_SPACE_COUNT_Z; ++z)
	{
		for (int x = 0; x < TILE_SPACE_COUNT_X; ++x)
		{
			int idx = x + z * TILE_SPACE_COUNT_X;
			if (idx & 1)
				continue;

			XMFLOAT3 offset = XMFLOAT3(x, 0, z);
			XMFLOAT3 pos = Multiply(offset, tileSpaceMat);
			playSpace[idx] = new PlaySpace(pos);

			// find out first tile arrange area
			float idDistX = abs(centerIDX - x);
			float idDistZ = abs(centerIDZ - z);
			float idDist = idDistX + idDistZ;
			if (idDist <= 4 && idDistX <= 2)
			{
				Tile* newTile = new Tile((unsigned int)idx);
				tiles.push_back(newTile);
				newTile->Move(idx, pos);
				playSpace[idx]->SetTile(newTile);

				// find out token places by tile index
				if (tileObjIdx == 0|| tileObjIdx == 13|| tileObjIdx == 15)
				{
					Token* newToken = new Token(idx, true);
					tokens.push_back(newToken);
					playSpace[idx]->SetToken(newToken);
					newToken->Move(idx,pos);
				}
				else if (tileObjIdx == 3|| tileObjIdx == 5|| tileObjIdx == 18)
				{
					Token* newToken = new Token(idx, false);
					tokens.push_back(newToken);
					playSpace[idx]->SetToken(newToken);
					newToken->Move(idx, pos);
				}
				tileObjIdx++;
			}
		}
	}

	redToken = new Token(true);
	greenToken = new Token(false);
	redTile = new Tile(true);
	greenTile = new Tile(false);

	isMove = false;
	holdingTokenID = NONE;
	holdingTileID = NONE;
}

NonagaStage::~NonagaStage()
{
	for (int i = 0; i < TILE_SPACE_COUNT_X * TILE_SPACE_COUNT_Z; ++i)
	{
		if (playSpace[i])
		{
			delete playSpace[i];
			playSpace[i] = nullptr;
		}
	}
	for (int i = 0; i < TOKEN_OBJ_COUNT_TOTAL; ++i)
	{
		delete tokens[i];
	}
	for (int i = 0; i < TILE_OBJ_COUNT; ++i)
	{
		delete tiles [i] ;
	}
	delete redToken;
	delete greenToken;
	delete redTile;
	delete greenTile;
}


bool NonagaStage::GetCurID2(const Geometrics::Ray& ray, const Geometrics::PlaneInf& detectPlane)
{
	XMFLOAT3 curTokenPickPt;
	Geometrics::IntersectRayPlaneInf(ray, detectPlane, &curTokenPickPt);

	XMFLOAT3 mTokenPt = Multiply(curTokenPickPt, invTileSpaceMat);
	mTokenPt += XMFLOAT3(0.5f, 0, 0.5f);
	int evenOdd = (int)mTokenPt.x & 1;

	XMINT2 pDetectID2 = XMINT2(mTokenPt.x, floor((mTokenPt.z - evenOdd + 0.5f) / 2.0f) * 2 + evenOdd);
	pDetectID = mTokenPt.x + (floor((mTokenPt.z - evenOdd + 0.5f) / 2.0f) * 2 + evenOdd) * TILE_SPACE_COUNT_X;

	return (0 <= pDetectID2.x && pDetectID2.x < TILE_SPACE_COUNT_X && 0 < pDetectID2.y && pDetectID2.y < TILE_SPACE_COUNT_Z);

}
bool NonagaStage::IsWin()
{
	return false;
}
void NonagaStage::TokenDragStart(const Geometrics::Ray ray)
{
	holdingTokenID = NONE;

	for (auto t : tokens)
	{
		t->UpdateBound();

		if (t->IsP1() == logic->IsP1Turn() && t->IsPicking(ray))
		{
			holdingTokenID = t->ID();

			return;
		}
	}
}
void NonagaStage::TokenDragging()
{
	redToken->SetEnabled(false);
	greenToken->SetEnabled(false);

	if (holdingTokenID == NONE)
		return;

	XMINT2 pDetectID2 = XMINT2(
		pDetectID % TILE_SPACE_COUNT_X,
		pDetectID / TILE_SPACE_COUNT_X);

	if (isMove = logic->CanMoveToken(holdingTokenID, pDetectID))
	{
		greenToken->SetEnabled(true);
		greenToken->Move(NONE, playSpace[pDetectID2.x + pDetectID2.y * TILE_SPACE_COUNT_X]->pos);
	}
	else
	{
		redToken->SetEnabled(true);
		redToken->Move(NONE, playSpace[pDetectID2.x + pDetectID2.y * TILE_SPACE_COUNT_X]->pos);
	}
}
void NonagaStage::TileDragStart(const Geometrics::Ray ray)
{
	holdingTileID = NONE;

	Tile* checkTile = playSpace[pDetectID]->GetTile();
	if (checkTile && logic->CanPickTile(checkTile->ID()))
	{
		holdingTileID = checkTile->ID();
	}
}
void NonagaStage::TileDragging()
{
	redTile->SetEnabled(false);
	greenTile->SetEnabled(false);

	if (holdingTileID == NONE)
		return;

	if (isMove = logic->CanMoveTileTo(holdingTileID, pDetectID))
	{
		Debugging::Instance()->Draw("Move !!!", 10, 10);
		greenTile->SetEnabled(true);
		greenTile->Move(NONE, playSpace[pDetectID]->pos);
	}
	else
	{
		Debugging::Instance()->Draw("No ~~", 10, 10);
		redTile->SetEnabled(true);
		redTile->Move(NONE, playSpace[pDetectID]->pos);
	}
}


void NonagaStage::Update(const Geometrics::Ray ray)
{
	if (!GetCurID2(ray, detectPlane))
		return;

	switch (curPlayState)
	{
	case PLAY_STATE_P1_TOKEN:
	{
		switch (Mouse::Instance()->LeftState())
		{
		case MOUSE_STATE_DOWN:
			TokenDragStart(ray);
			break;
		case MOUSE_STATE_PRESSING:
			TokenDragging();
			break;
		case MOUSE_STATE_UP:

			redToken->SetEnabled(false);
			greenToken->SetEnabled(false);

			if (holdingTokenID == NONE || !isMove)
				return;

			logic->TokenMove(holdingTokenID, pDetectID);

			curPlayState = PLAY_STATE_P1_TILE;

			if (IsWin())
				curPlayState = PLAY_STATE_FINISH;
			
			break;
		}
	}
		break;
	case PLAY_STATE_P1_TILE:
	{
		switch (Mouse::Instance()->LeftState())
		{
		case MOUSE_STATE_DOWN:
			TileDragStart(ray);
			break;
		case MOUSE_STATE_PRESSING:
			TileDragging();
			break;
		case MOUSE_STATE_UP:
		{
			redTile->SetEnabled(false);
			greenTile->SetEnabled(false);

			if (holdingTileID == NONE || !isMove)
				break;

			logic->TileMove(holdingTileID, pDetectID);

			curPlayState = PLAY_STATE_P2_TOKEN;
		}
			break;
		}
	}
		break;
	case PLAY_STATE_P2_TOKEN:
	{
		switch (Mouse::Instance()->LeftState())
		{
		case MOUSE_STATE_DOWN:
			TokenDragStart(ray);
			break;
		case MOUSE_STATE_PRESSING:
			TokenDragging();
			break;
		case MOUSE_STATE_UP:

			redToken->SetEnabled(false);
			greenToken->SetEnabled(false);

			if (holdingTokenID == NONE || !isMove)
				return;

			logic->TokenMove(holdingTokenID, pDetectID);

			curPlayState = PLAY_STATE_P1_TILE;

			if (IsWin())
				curPlayState = PLAY_STATE_FINISH;

			break;
		}
	}
	break;
	case PLAY_STATE_P2_TILE:
	{
		switch (Mouse::Instance()->LeftState())
		{
		case MOUSE_STATE_DOWN:
			TileDragStart(ray);
			break;
		case MOUSE_STATE_PRESSING:
			TileDragging();
			break;
		case MOUSE_STATE_UP:
		{
			redTile->SetEnabled(false);
			greenTile->SetEnabled(false);

			if (holdingTileID == NONE || !isMove)
				break;

			logic->TileMove(holdingTileID, pDetectID);

			curPlayState = PLAY_STATE_P2_TOKEN;
		}
		break;
		}
	}
	break;
	case PLAY_STATE_FINISH:
		break;
	}
}

void NonagaStage::Objs(std::vector<Object*>& objOutput)
{
	for (int i = 0; i < TILE_OBJ_COUNT; ++i)
	{
		objOutput.push_back(tiles[i]->Obj());
	}
	for (int i = 0; i < TOKEN_OBJ_COUNT_TOTAL; ++i)
	{
		objOutput.push_back(tokens[i]->Obj());
	}
}

void NonagaStage::Render(const XMMATRIX& vp, XMFLOAT3 eye, unsigned int sceneDepth)const
{
	redTile->Render(vp, eye, sceneDepth);
	greenTile->Render(vp, eye, sceneDepth);
	redToken->Render(vp, eye, sceneDepth);
	greenToken->Render(vp, eye, sceneDepth);
}


NonagaLogic::NonagaLogic(PlaySpace* const* space)
	:space(space)
{
	unmovableTileID = NONE;
	p1Turn = true;
}

NonagaLogic::~NonagaLogic()
{
}


bool NonagaLogic::CheckDirection(XMINT2 holdingTokenID2, XMINT2 offset, XMINT2& destId2)
{
	XMINT2 lastId2 = XMINT2(-1, -1);
	XMINT2 curId2 = holdingTokenID2;

	for (int i = 0; i < fmaxf(TILE_SPACE_COUNT_X, TILE_SPACE_COUNT_Z); ++i)
	{
		curId2.x += offset.x;
		curId2.y += offset.y;

		if (curId2.x < 0 || curId2.x >= TILE_SPACE_COUNT_X || curId2.y<0 || curId2.y >= TILE_SPACE_COUNT_Z)
			break;
		UINT idx = curId2.x + curId2.y * TILE_SPACE_COUNT_X;
		if (space[idx]->GetTile() == nullptr ||
			space[idx]->GetToken())
			break;

		lastId2.x = curId2.x;
		lastId2.y = curId2.y;
	}

	destId2.x = lastId2.x;
	destId2.y = lastId2.y;

	return (curId2.x >= 0 && curId2.y >= 0);
}
int NonagaLogic::CanMoveToken(int from, int to)
{
	XMINT2 fromID2 = XMINT2(from % TILE_SPACE_COUNT_X, from / TILE_SPACE_COUNT_X);
	XMINT2 toID2 = XMINT2(to % TILE_SPACE_COUNT_X, to / TILE_SPACE_COUNT_X);

	XMINT2 destId2;
	if (toID2.y > fromID2.y)
	{
		// up right
		if (toID2.x > fromID2.x)
		{
			if (!CheckDirection(fromID2, XMINT2(1, 1), destId2))
				return 0;
		}
		// up left
		else if (toID2.x < fromID2.x)
		{
			if (!CheckDirection(fromID2, XMINT2(-1, 1), destId2))
				return 0;
		}
		// up
		else
		{
			if (!CheckDirection(fromID2, XMINT2(0, 2), destId2))
				return 0;
		}
	}
	else if (toID2.y < fromID2.y)
	{
		// down right
		if (toID2.x > fromID2.x)
		{
			if (!CheckDirection(fromID2, XMINT2(1, -1), destId2))
				return 0;
		}
		// down left
		else if (toID2.x < fromID2.x)
		{
			if (!CheckDirection(fromID2, XMINT2(-1, -1), destId2))
				return 0;
		}
		// down
		else
		{
			if (!CheckDirection(fromID2, XMINT2(0, -2), destId2))
				return 0;
		}
	}
	else
		return 0;

	return ((destId2.x == toID2.x) && (destId2.y == toID2.y));
}
int NonagaLogic::CheckNeighbor(int centerID, int exceptID)
{
	XMINT2 centerID2 = XMINT2(
		centerID % TILE_SPACE_COUNT_X,
		centerID / TILE_SPACE_COUNT_X);
	XMINT2 exceptID2 = XMINT2(
		exceptID % TILE_SPACE_COUNT_X,
		exceptID / TILE_SPACE_COUNT_X);

	int count = 0;
	for (int z = centerID2.y - 2; z <= centerID2.y + 2; ++z)
	{
		for (int x = centerID2.x - 1; x <= centerID2.x + 1; ++x)
		{
			if (x < 0 || x >= TILE_SPACE_COUNT_X || z < 0 || z >= TILE_SPACE_COUNT_Z || (x == exceptID2.x && z == exceptID2.y))
				continue;

			int idx = x + z * TILE_SPACE_COUNT_X;
			if (space[idx] && space[idx]->GetTile())
				count++;
		}
	}

	return count;
}
bool NonagaLogic::CanPickTile(int id)
{
	return (
		(unmovableTileID != id) &&
		(space[id]->GetToken() == nullptr) &&
		(CheckNeighbor(id, id) <= 4));

}
bool NonagaLogic::CanMoveTileTo(int from, int to)
{
	return (!space[to]->GetTile() && CheckNeighbor(to, from) >= 2);
}
void NonagaLogic::TokenMove(int from, int to)
{
	XMFLOAT3 toPos = space[to]->pos;
	Token* token = space[from]->GetToken();

	token->Move(to, toPos);
	space[from]->SetToken(nullptr);
	space[to]->SetToken(token);
}
void NonagaLogic::TileMove(int from, int to)
{
	unmovableTileID = to;
	Tile* movingTile = space[from]->GetTile();
	space[from]->SetTile(nullptr);
	space[to]->SetTile(movingTile);

	movingTile->Move(to, space[to]->pos);

	p1Turn = !p1Turn;
}
int NonagaLogic::GetScore(const std::vector<Token*>& tokens)
{
	int score = 0;

	for (int i = 0; i < TOKEN_OBJ_COUNT_PER; ++i)
	{
		Token* curToken = tokens[i + (p1Turn ? 0 : 3)];

		XMUINT2 curTokenID2 = XMUINT2(
			curToken->ID() % TILE_SPACE_COUNT_X,
			curToken->ID() / TILE_SPACE_COUNT_X);

		for (int z = curTokenID2.y - 2; z <= curTokenID2.y + 2; ++z)
		{
			for (int x = curTokenID2.x - 1; x <= curTokenID2.x + 1; ++x)
			{
				int checkIdx = x + z * TILE_SPACE_COUNT_X;

				if (z < 0 || z >= TILE_SPACE_COUNT_Z || x < 0 || x >= TILE_SPACE_COUNT_X)continue;
				if (space[checkIdx] == nullptr) continue;
				if ((z == curTokenID2.y && x == curTokenID2.x)) continue;
				if (space[curToken->ID()]->GetToken() == nullptr || space[checkIdx]->GetToken() == nullptr)continue;
				if (space[curToken->ID()]->GetToken()->IsP1() != space[checkIdx]->GetToken()->IsP1())continue;

				score++;
			}
		}
	}

	return score;
}