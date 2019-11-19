#include "NonagaLogic.h"
#include "Debugging.h"
#include "CommObjInfo.h"
#include "Mouse.h"
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
	holdingTokenObjID = OBJ_PICKING_NONE;
	holdingTileObjID = OBJ_PICKING_NONE;
	curIdx = OBJ_PICKING_NONE;
	holdingSpaceID = OBJ_PICKING_NONE;

	p1Turn = true;
}

NonagaLogic::~NonagaLogic()
{
	for (int i = 0; i < TILE_SPACE_COUNT_X * TILE_SPACE_COUNT_Z; ++i)
	{
		delete playSpace[i];
	}

	delete[] playSpace;
}

void NonagaLogic::SetupFirstArrange()
{
	std::vector<XMFLOAT3> firstTileArrange(TILE_OBJ_COUNT);
	std::vector<XMFLOAT3> firstTokenArrange(TOKEN_OBJ_COUNT_TOTAL);
	playSpace = new PlaySpace* [TILE_SPACE_COUNT_Z * TILE_SPACE_COUNT_X]{ nullptr };

	// create tile space
	int centerIDZ = TILE_SPACE_COUNT_Z / 2.0f;
	int centerIDX = TILE_SPACE_COUNT_X / 2.0f;

	tileSpaceMat = XMMATRIX(
		TILE_SPACE_INTERVAL_X, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, TILE_SPACE_INTERVAL_Z / 2.0f, 0,
		-TILE_SPACE_INTERVAL_X * centerIDX, 0, -TILE_SPACE_INTERVAL_Z * centerIDZ / 2.0f, 1);

	invTileSpaceMat = XMMatrixInverse(&XMMatrixDeterminant(tileSpaceMat), tileSpaceMat);

	tileDetectPlane = Geometrics::PlaneInf(XMFLOAT3(0, 0, 0), UP);
	tokenDetectPlane = Geometrics::PlaneInf(XMFLOAT3(0, 5, 0), UP);

	int tileObjIdx = 0;
	std::vector<int> p1Id;
	std::vector<int> p2Id;

	for (int z = 0; z < TILE_SPACE_COUNT_Z; ++z)
	{
		for (int x = 0; x < TILE_SPACE_COUNT_X; ++x)
		{
			int idx = x + z * TILE_SPACE_COUNT_X;
			if (idx & 1)
				continue;

			XMFLOAT3 offset = XMFLOAT3(x, 0, z);
			XMFLOAT3 pos = Multiply(offset, tileSpaceMat);
			playSpace[idx] = new PlaySpace(pos, TILE_STATE_NONE);

			// find out first tile arrange area
			float idDistX = abs(centerIDX - x);
			float idDistZ = abs(centerIDZ - z);
			float idDist = idDistX + idDistZ;
			if (idDist <= 4 && idDistX <= 2)
			{
				firstTileArrange[tileObjIdx]=playSpace[idx]->pos;

				playSpace[idx]->state = TILE_STATE_TILE;

				// find out token places by tile index
				if (tileObjIdx == 0 || tileObjIdx == 13 || tileObjIdx == 15)
				{
					playSpace[idx]->state = TILE_STATE_P1;
					p1Id.push_back(idx);
				}
				else if (tileObjIdx == 3 || tileObjIdx == 5 || tileObjIdx == 18)
				{
					playSpace[idx]->state = TILE_STATE_P2;
					p2Id.push_back(idx);
				}
				tileObjIdx++;
			}
		}
	}

	/*playSpace[p1Id[0]]->tokenID = 0;
	playSpace[p1Id[1]]->tokenID = 1;
	playSpace[p1Id[2]]->tokenID = 2;
	playSpace[p2Id[0]]->tokenID = 3;
	playSpace[p2Id[1]]->tokenID = 4;
	playSpace[p2Id[2]]->tokenID = 5;*/
	firstTokenArrange[0] = playSpace[p1Id[0]]->pos;
	firstTokenArrange[1] = playSpace[p1Id[1]]->pos;
	firstTokenArrange[2] = playSpace[p1Id[2]]->pos;
	firstTokenArrange[3] = playSpace[p2Id[0]]->pos;
	firstTokenArrange[4] = playSpace[p2Id[1]]->pos;
	firstTokenArrange[5] = playSpace[p2Id[2]]->pos;

	Notify(OBS_TILE_FIRST_ARRANGE, firstTileArrange.data());
	Notify(OBS_TOKEN_FIRST_ARRANGE, firstTokenArrange.data());

	p1Turn = true;
}



void NonagaLogic::CheckDirection(XMUINT2 id2, XMINT2 offset)
{
	UINT destIdx = -1;

	while (true)
	{
		id2.x += offset.x;
		id2.y += offset.y;

		if (id2.x >= TILE_SPACE_COUNT_X || id2.y >= TILE_SPACE_COUNT_Z)
			break;
		UINT idx = id2.x + id2.y * TILE_SPACE_COUNT_X;
		if (playSpace[idx]->state != TILE_STATE_TILE)
			break;

		destIdx = idx;
	}

	if (destIdx != -1)
		tempDest.insert(destIdx);
}
void NonagaLogic::CalcDests(XMUINT2 id2)
{
	tempDest.clear();

	// diagonal / upright dir
	CheckDirection(id2, XMINT2(1, 1));
	// diagonal / downleft dir
	CheckDirection(id2, XMINT2(-1, -1));
	// diagonal \ upleft dir
	CheckDirection(id2, XMINT2(-1, 1));
	// diagonal \ downright dir
	CheckDirection(id2, XMINT2(1, -1));
	// vertical up dir
	CheckDirection(id2, XMINT2(0, 2));
	// vertical down dir
	CheckDirection(id2, XMINT2(0, -2));
}

bool NonagaLogic::GetCurID2(const Geometrics::Ray& ray, const Geometrics::PlaneInf& detectPlane, XMUINT2* id)
{
	XMFLOAT3 curTokenPickPt;
	Geometrics::IntersectRayPlaneInf(ray, detectPlane, &curTokenPickPt);

	XMFLOAT3 mTokenPt = Multiply(curTokenPickPt, invTileSpaceMat);
	mTokenPt += XMFLOAT3(0.5f, 0, 0.5f);
	int evenOdd = (int)mTokenPt.x & 1;

	XMUINT2 tempID = XMUINT2(mTokenPt.x, floor((mTokenPt.z - evenOdd + 0.5f) / 2.0f) * 2 + evenOdd);
	if(id)
		*id = tempID;

	return (0 <= tempID.x || tempID.x < TILE_SPACE_COUNT_X || 0 < tempID.y || tempID.y < TILE_SPACE_COUNT_Z);

}
void NonagaLogic::TokenDragStart(bool range, unsigned int curTokenID, XMUINT2 curId2)
{
	holdingTokenObjID = curTokenID;

	if (holdingTokenObjID == OBJ_PICKING_NONE ||
		(p1Turn && curTokenID >= 3) ||
		(!p1Turn && curTokenID <= 2))
	{
		holdingTokenObjID = OBJ_PICKING_NONE;
	}
	else if(range)
	{
		CalcDests(curId2);
	}
}

void NonagaLogic::TokenDragging(bool range, XMUINT2 curId2)
{
	if (holdingTokenObjID != OBJ_PICKING_NONE)
	{
		UINT obsID = OBS_TOKEN_DISABLE_INDICATOR;
		XMFLOAT3* obsData = nullptr;

		if (range)
		{
			curIdx = curId2.x + curId2.y * TILE_SPACE_COUNT_X;

			if (playSpace[curIdx]->state == TILE_STATE_TILE)
			{
				if (tempDest.count(curIdx))
				{
					obsID = OBS_TOKEN_ENABLE_INDICATOR_GREEN;
					obsData = &playSpace[curIdx]->pos;
				}
				else
				{
					obsID = OBS_TOKEN_ENABLE_INDICATOR_RED;
					obsData = &playSpace[curIdx]->pos;
				}
			}

		}

		Notify(obsID, obsData);
	}
}
void NonagaLogic::TokenMove()
{
	if (holdingTokenObjID != OBJ_PICKING_NONE && tempDest.count(curIdx))
	{
		XMFLOAT4 data = XMFLOAT4(
			playSpace[curIdx]->pos.x,
			playSpace[curIdx]->pos.y,
			playSpace[curIdx]->pos.z,
			holdingTokenObjID);
		Notify(OBS_TOKEN_MOVE, &data);

		playSpace[holdingSpaceID]->state = TILE_STATE_TILE;
		playSpace[curIdx]->state = p1Turn? TILE_STATE_P1 : TILE_STATE_P2;
	}
	else
	{
		Notify(OBS_TOKEN_DISABLE_INDICATOR, nullptr);
	}
}
void NonagaLogic::TileDragStart(bool range, unsigned int curTokenID, XMUINT2 curId2)
{
	holdingTileObjID = curTokenID;

	if (holdingTileObjID == OBJ_PICKING_NONE)
	{
		
	}
	else if (range)
	{
		CalcDests(curId2);
	}
}
void NonagaLogic::TileDragging(bool range)
{
}
void NonagaLogic::TileMove()
{
}
void NonagaLogic::Update(const Geometrics::Ray ray, unsigned int curTokenID, unsigned int curTileID)
{

	switch (curPlayState)
	{
	case PLAY_STATE_P1_TOKEN:
	{
		XMUINT2 curID2;
		const bool isInRange = GetCurID2(ray, tokenDetectPlane, &curID2);
		curIdx = curID2.x + curID2.y * TILE_SPACE_COUNT_X;

		switch (Mouse::Instance()->LeftState())
		{
		case MOUSE_STATE_DOWN:
			holdingSpaceID = curIdx;
			TokenDragStart(isInRange, curTokenID, curID2);
			break;
		case MOUSE_STATE_PRESSING:
			TokenDragging(isInRange, curID2);
			break;
		case MOUSE_STATE_UP:
			TokenMove();

			curIdx = OBJ_PICKING_NONE;
			holdingTokenObjID = OBJ_PICKING_NONE;
			holdingSpaceID = OBJ_PICKING_NONE;
			curPlayState = PLAY_STATE_P1_TILE;
			break;
		}
	}
		break;
	case PLAY_STATE_P1_TILE:
	{
		XMUINT2 curID2;
		const bool isInRange = GetCurID2(ray, tileDetectPlane, &curID2);
		curIdx = curID2.x + curID2.y * TILE_SPACE_COUNT_X;

		switch (Mouse::Instance()->LeftState())
		{
		case MOUSE_STATE_DOWN:
			TokenDragStart(isInRange, curTokenID, curID2);
			break;
		case MOUSE_STATE_PRESSING:
			break;
		case MOUSE_STATE_UP:
			TokenMove();

			curIdx = OBJ_PICKING_NONE;
			holdingTokenObjID = OBJ_PICKING_NONE;
			curPlayState = PLAY_STATE_P1_TILE;
			break;
		}
	}
		break;
	case PLAY_STATE_P2_TOKEN:
		break;
	case PLAY_STATE_P2_TILE:
		break;
	}



}


