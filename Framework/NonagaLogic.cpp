#include "NonagaLogic.h"
#include "Debugging.h"
#include "Mouse.h"
#include "Token.h"
#include "Tile.h"
#include "PlaySpace.h"
#include "Transform.h"
#include "Object.h"
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

NonagaLogic::NonagaLogic()
{
	curPlayState = PLAY_STATE_P1_TOKEN;
	holdingToken = nullptr;
	holdingTile = nullptr;


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
				tiles[tileObjIdx] = new Tile(playSpace, idx);
				tiles[tileObjIdx]->Move(idx);

				// find out token places by tile index
				if (tileObjIdx == 0)		tokens[0] = new Token(playSpace, idx, true);
				else if (tileObjIdx == 13)	tokens[1] = new Token(playSpace, idx, true);
				else if (tileObjIdx == 15)	tokens[2] = new Token(playSpace, idx, true);
				else if (tileObjIdx == 3)	tokens[3] = new Token(playSpace, idx, false);
				else if (tileObjIdx == 5)	tokens[4] = new Token(playSpace, idx, false);
				else if (tileObjIdx == 18)	tokens[5] = new Token(playSpace,idx, false);
				tileObjIdx++;
			}
		}
	}

	redToken = new Token(true);
	greenToken = new Token(false);
	redTile = new Tile(true);
	greenTile = new Tile(false);

	p1Turn = true;
	isMove = false;
	unmovableTileID = OBJ_PICKING_NONE;
}

NonagaLogic::~NonagaLogic()
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


bool NonagaLogic::GetCurID2(const Geometrics::Ray& ray, const Geometrics::PlaneInf& detectPlane)
{
	XMFLOAT3 curTokenPickPt;
	Geometrics::IntersectRayPlaneInf(ray, detectPlane, &curTokenPickPt);

	XMFLOAT3 mTokenPt = Multiply(curTokenPickPt, invTileSpaceMat);
	mTokenPt += XMFLOAT3(0.5f, 0, 0.5f);
	int evenOdd = (int)mTokenPt.x & 1;

	pDetectID2 = XMINT2(mTokenPt.x, floor((mTokenPt.z - evenOdd + 0.5f) / 2.0f) * 2 + evenOdd);

	return (0 <= pDetectID2.x && pDetectID2.x < TILE_SPACE_COUNT_X && 0 < pDetectID2.y && pDetectID2.y < TILE_SPACE_COUNT_Z);

}


bool CheckDirection(PlaySpace* const* playSpace, XMINT2 holdingTokenID2, XMINT2 offset, OUT XMINT2& destId2)
{
	XMINT2 lastId2 = XMINT2(-1, -1);
	XMINT2 curId2 = holdingTokenID2;

	for(int i=0; i<fmaxf(TILE_SPACE_COUNT_X, TILE_SPACE_COUNT_Z); ++i)
	{
		curId2.x += offset.x;
		curId2.y += offset.y;

		if (curId2.x >= TILE_SPACE_COUNT_X || curId2.y >= TILE_SPACE_COUNT_Z)
			break;
		UINT idx = curId2.x + curId2.y * TILE_SPACE_COUNT_X;
		if (playSpace[idx]->State() != TILE_STATE_TILE)
			break;

		lastId2.x = curId2.x;
		lastId2.y = curId2.y;
	}

	destId2.x = lastId2.x;
	destId2.y = lastId2.y;

	return (curId2.x>=0 && curId2.y>=0);
}
bool NonagaLogic::CanMove()
{
	UINT holdingTokenID = holdingToken->ID();

	XMINT2 curHoldingID2 = XMINT2(holdingTokenID % TILE_SPACE_COUNT_X, holdingTokenID / TILE_SPACE_COUNT_X);
	
	XMINT2 destId2;
	if (pDetectID2.y > curHoldingID2.y)
	{
		// up right
		if (pDetectID2.x > curHoldingID2.x)
		{
			if (!CheckDirection(playSpace, curHoldingID2, XMINT2(1, 1), destId2))
				return false;
		}
		// up left
		else if (pDetectID2.x < curHoldingID2.x)
		{
			if (!CheckDirection(playSpace, curHoldingID2, XMINT2(-1, 1), destId2))
				return false;
		}
		// up
		else
		{
			if (!CheckDirection(playSpace, curHoldingID2, XMINT2(0, 2), destId2))
				return false;
		}
	}
	else if (pDetectID2.y < curHoldingID2.y)
	{
		// down right
		if (pDetectID2.x > curHoldingID2.x)
		{
			if (!CheckDirection(playSpace, curHoldingID2, XMINT2(1, -1), destId2))
				return false;
		}
		// down left
		else if (pDetectID2.x < curHoldingID2.x)
		{
			if (!CheckDirection(playSpace, curHoldingID2, XMINT2(-1, -1), destId2))
				return false;
		}
		// down
		else
		{
			if (!CheckDirection(playSpace, curHoldingID2, XMINT2(0, -2), destId2))
				return false;
		}
	}
	else // same index
		return false;

	return ((destId2.x == pDetectID2.x) && (destId2.y == pDetectID2.y));
}

int CheckNeighbor(PlaySpace** playSpace, XMINT2 id2, XMINT2 except)
{
	int count = 0;
	for (int z = id2.y - 2; z <= id2.y + 2; ++z)
	{
		for (int x = id2.x - 1; x <= id2.x + 1; ++x)
		{
			if (x < 0 || x >= TILE_SPACE_COUNT_X || z <0 || z>=TILE_SPACE_COUNT_Z || (x== except.x&&z== except.y))
				continue;

			int idx = x + z * TILE_SPACE_COUNT_X;
			if(playSpace[idx] && playSpace[idx]->State()!=TILE_STATE_NONE)
				count++;
		}
	}

	return count;
}
bool NonagaLogic::CanMoveTile(unsigned int toID)
{
	XMINT2 curHoldingID2 = XMINT2(holdingTile->ID() % TILE_SPACE_COUNT_X, holdingTile->ID() / TILE_SPACE_COUNT_X);
	XMINT2 toID2 = XMINT2(toID % TILE_SPACE_COUNT_X, toID / TILE_SPACE_COUNT_X);

	XMINT2 destId2;
	if (toID2.y > curHoldingID2.y)
	{
		// up right
		if (toID2.x > curHoldingID2.x)
		{
			if (!CheckDirection(playSpace, curHoldingID2, XMINT2(1, 1), destId2))
				return false;
		}
		// up left
		else if (toID2.x < curHoldingID2.x)
		{
			if (!CheckDirection(playSpace, curHoldingID2, XMINT2(-1, 1), destId2))
				return false;
		}
		// up
		else
		{
			if (!CheckDirection(playSpace, curHoldingID2, XMINT2(0, 2), destId2))
				return false;
		}
	}
	else if (toID2.y < curHoldingID2.y)
	{
		// down right
		if (toID2.x > curHoldingID2.x)
		{
			if (!CheckDirection(playSpace, curHoldingID2, XMINT2(1, -1), destId2))
				return false;
		}
		// down left
		else if (toID2.x < curHoldingID2.x)
		{
			if (!CheckDirection(playSpace, curHoldingID2, XMINT2(-1, -1), destId2))
				return false;
		}
		// down
		else
		{
			if (!CheckDirection(playSpace, curHoldingID2, XMINT2(0, -2), destId2))
				return false;
		}
	}
	else // same index
		return false;

	return ((destId2.x == toID2.x) && (destId2.y == toID2.y));
}

int NonagaLogic::GetScore()
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
				if (playSpace[checkIdx] == nullptr) continue;
				if((z == curTokenID2.y && x == curTokenID2.x)) continue;
				if (playSpace[curToken->ID()]->State() != playSpace[checkIdx]->State())continue;

				score++;
			}
		}
	}

	return score;
}
bool NonagaLogic::IsWin()
{
	return false;
}

void NonagaLogic::TokenDragStart(const Geometrics::Ray ray)
{
	holdingToken = nullptr;

	for (auto t : tokens)
	{
		t->UpdateBound();

		if (t->IsP1() == p1Turn && t->IsPicking(ray))
		{
			holdingToken = t;

			return;
		}
	}
}
void NonagaLogic::TokenDragging()
{
	redToken->SetEnabled(false);
	greenToken->SetEnabled(false);

	if (holdingToken == nullptr)
		return;

	if (isMove=CanMove())
	{
		greenToken->SetEnabled(true);
		greenToken->RawMove(playSpace[pDetectID2.x + pDetectID2.y * TILE_SPACE_COUNT_X]->pos);
	}
	else
	{
		redToken->SetEnabled(true);
		redToken->RawMove(playSpace[pDetectID2.x + pDetectID2.y * TILE_SPACE_COUNT_X]->pos);
	}
}
void NonagaLogic::TokenMove()
{
	redToken->SetEnabled(false);
	greenToken->SetEnabled(false);

	if (holdingToken == nullptr || !isMove)
		return;

	holdingToken->Move(pDetectID2.x + pDetectID2.y*TILE_SPACE_COUNT_X);

	curPlayState = PLAY_STATE_P1_TILE;
	
}
void NonagaLogic::TileDragStart(const Geometrics::Ray ray)
{
	holdingTile = nullptr;

	int curDId = pDetectID2.x + pDetectID2.y * TILE_SPACE_COUNT_X;
	if ((4 < CheckNeighbor(playSpace, pDetectID2, pDetectID2)) ||
		playSpace[curDId]->State() != TILE_STATE_TILE||
		unmovableTileID == curDId)
		return;

	for (auto t : tiles)
	{
		t->UpdateBound();

		if (t->IsPicking(ray))
		{
			holdingTile = t;

			break;
		}
	}
}
void NonagaLogic::TileDragging()
{
	redTile->SetEnabled(false);
	greenTile->SetEnabled(false);

	if (holdingTile == nullptr)
		return;

	int idx = pDetectID2.x + pDetectID2.y * TILE_SPACE_COUNT_X;
	XMINT2 holdingIdx = XMINT2(
		holdingTile->ID() % TILE_SPACE_COUNT_X,
		holdingTile->ID() / TILE_SPACE_COUNT_X);

	if (isMove = (playSpace[idx]->State()==TILE_STATE_NONE && CheckNeighbor(playSpace, pDetectID2, holdingIdx)>=2))
	{
		greenTile->SetEnabled(true);
		greenTile->RawMove(playSpace[idx]->pos);
	}
	else
	{
		redTile->SetEnabled(true);
		redTile->RawMove(playSpace[idx]->pos);
	}
}
void NonagaLogic::TileMove()
{
	redTile->SetEnabled(false);
	greenTile->SetEnabled(false);

	int curDId = pDetectID2.x + pDetectID2.y * TILE_SPACE_COUNT_X;
	if (holdingTile == nullptr || !isMove)
		return;

	unmovableTileID = curDId;
	holdingTile->Move(curDId);

	p1Turn = !p1Turn;
	curPlayState = PLAY_STATE_P2_TOKEN;
}
void NonagaLogic::Update(const Geometrics::Ray ray)
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
			TokenMove();
			if (IsWin())
				curPlayState = PLAY_STATE_FINISH;
			holdingToken = nullptr;
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
			TileMove();

			holdingTile = nullptr;
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
			TokenMove();
			if (IsWin())
				curPlayState = PLAY_STATE_FINISH;
			
			holdingToken = nullptr;
			break;
		}
	}
		break;
	case PLAY_STATE_P2_TILE:
		switch (Mouse::Instance()->LeftState())
		{
		case MOUSE_STATE_DOWN:
			TileDragStart(ray);
			break;
		case MOUSE_STATE_PRESSING:
			TileDragging();
			break;
		case MOUSE_STATE_UP:
			TileMove();

			holdingTile = nullptr;
			break;
		}
		break;
	case PLAY_STATE_FINISH:
		break;
	}



}

void NonagaLogic::Render(const XMMATRIX& vp, XMFLOAT3 eye, unsigned int sceneDepth)
{
	for (int i=0; i<TILE_OBJ_COUNT; ++i)
	{
		tiles[i]->Render(vp, eye, sceneDepth);
	}
	for (int i = 0; i < TOKEN_OBJ_COUNT_TOTAL; ++i)
	{
		tokens[i]->Render(vp, eye, sceneDepth);
	}
	redTile->Render(vp, eye, sceneDepth);
	greenTile->Render(vp, eye, sceneDepth);
	redToken->Render(vp, eye, sceneDepth);
	greenToken->Render(vp, eye, sceneDepth);
}


