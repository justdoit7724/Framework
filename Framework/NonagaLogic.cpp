#include "NonagaLogic.h"
#include "Debugging.h"

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
#define TILE_SPACE_INTERVAL_Z 10.0f
#define TILE_OBJ_COUNT 19

NonagaLogic::NonagaLogic(
	std::vector<XMFLOAT3>* firstTileArrange,
	std::vector<XMFLOAT3>* firstTokenArrange)
{
	firstTileArrange->clear();
	firstTokenArrange->clear();

	curPlayState = PLAY_STATE_P1_TOKEN;

	playSpace = new PlaySpace * [TILE_SPACE_COUNT_Z * TILE_SPACE_COUNT_X]{ nullptr };

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
	tokenDetectPlane = Geometrics::PlaneInf(XMFLOAT3(0, 4, 0), UP);

	int tileObjIdx = -1;
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
			playSpace[idx] = new PlaySpace(Multiply(offset, tileSpaceMat));
			 
			// find out first tile arrange area
			float idDistX = abs(centerIDX - x);
			float idDistZ = abs(centerIDZ - z);
			float idDIst = idDistX + idDistZ;
			if (idDistX <= 2 && idDIst <= 4)
			{
				firstTileArrange->push_back(playSpace[idx]->pos);

				tileObjIdx++;
				playSpace[idx]->tileID = tileObjIdx;
				playSpace[idx]->state |= TILE_STATE_TILE;

				// find out token places by tile index
				if (tileObjIdx == 0|| tileObjIdx == 13|| tileObjIdx == 15)
				{
					p1Id.push_back(idx);
				}
				else if (tileObjIdx == 3|| tileObjIdx == 5|| tileObjIdx == 18)
				{
					p2Id.push_back(idx);
				}
			}
		}
	}
	assert(firstTileArrange->size() == TILE_OBJ_COUNT);


	playSpace[p1Id[0]]->state |= TILE_STATE_P1;
	playSpace[p1Id[0]]->tokenID = 0;
	playSpace[p1Id[1]]->state |= TILE_STATE_P1;
	playSpace[p1Id[1]]->tokenID = 1;
	playSpace[p1Id[2]]->state |= TILE_STATE_P1;
	playSpace[p1Id[2]]->tokenID = 2;
	playSpace[p2Id[0]]->state |= TILE_STATE_P2;
	playSpace[p2Id[0]]->tokenID = 3;
	playSpace[p2Id[1]]->state |= TILE_STATE_P2;
	playSpace[p2Id[1]]->tokenID = 4;
	playSpace[p2Id[2]]->state |= TILE_STATE_P2;
	playSpace[p2Id[2]]->tokenID = 5;
	firstTokenArrange->push_back(playSpace[p1Id[0]]->pos);
	firstTokenArrange->push_back(playSpace[p1Id[1]]->pos);
	firstTokenArrange->push_back(playSpace[p1Id[2]]->pos);
	firstTokenArrange->push_back(playSpace[p2Id[0]]->pos);
	firstTokenArrange->push_back(playSpace[p2Id[1]]->pos);
	firstTokenArrange->push_back(playSpace[p2Id[2]]->pos);
}

NonagaLogic::~NonagaLogic()
{
	for (int i = 0; i < TILE_SPACE_COUNT_X * TILE_SPACE_COUNT_Z; ++i)
	{
		delete playSpace[i];
	}

	delete[] playSpace;
}

XMINT2 NonagaLogic::Convert2ID(XMFLOAT3 pickPt)
{
	XMFLOAT3 mTokenPt = Multiply(pickPt, invTileSpaceMat);
	int evenOdd = (int)mTokenPt.x & 1;

	return XMINT2(mTokenPt.x, floor((mTokenPt.z - evenOdd + 0.5f)/2.0f)*2 + evenOdd);

}
void NonagaLogic::Update(const Geometrics::Ray ray)
{
	switch (curPlayState)
	{
	case NonagaLogic::PLAY_STATE_P1_TOKEN:
	{
		XMFLOAT3 curTokenPickPt;
		Geometrics::IntersectRayPlaneInf(ray, tokenDetectPlane, &curTokenPickPt);
		
		XMINT2 pickID = Convert2ID(curTokenPickPt);
		Debugging::Instance()->Draw("pt x ID = ", pickID.x, 10, 10);
		if (pickID.x <0 || pickID.x >= TILE_SPACE_COUNT_X || pickID.y <0 || pickID.y >=TILE_SPACE_COUNT_Z)
			return;
		UINT idx = pickID.x + pickID.y * TILE_SPACE_COUNT_X;
		if (playSpace[idx] == nullptr)
			return;
	}
		break;
	case NonagaLogic::PLAY_STATE_P1_TILE:
	{
		XMFLOAT3 curTilePickPt;
		Geometrics::IntersectRayPlaneInf(ray, tileDetectPlane, &curTilePickPt);
		XMFLOAT3 mTilePt = Multiply(curTilePickPt, invTileSpaceMat);

	}
		break;
	case NonagaLogic::PLAY_STATE_P2_TOKEN:
		break;
	case NonagaLogic::PLAY_STATE_P2_TILE:
		break;
	}



}

void NonagaLogic::MoveToken(unsigned int* id, XMFLOAT3* pos)
{
}

void NonagaLogic::MoveTile(unsigned int* id, XMFLOAT3* pos)
{
}

bool NonagaLogic::PreviewToken(bool* isPossible, XMFLOAT3* pos)
{
	return false;
}

bool NonagaLogic::PreviewTile(bool* isPossible, XMFLOAT3* pos)
{
	return false;
}
