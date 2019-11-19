#pragma once
#include "Geometrics.h"
#include "ObserverDP.h"

class NonagaLogic : public Subject
{
public:
	NonagaLogic();
	~NonagaLogic();

	void SetupFirstArrange();

	void Update(const Geometrics::Ray ray, unsigned int curTokenID, unsigned int curTileID);
private:
	void TokenDragStart(bool range, unsigned int curTokenID, XMUINT2 curId2);
	void TokenDragging(bool range, XMUINT2 curId2);
	void TokenMove();
	void TileDragStart(bool range, unsigned int curTokenID, XMUINT2 curId2);
	void TileDragging(bool range);
	void TileMove();
	void CheckDirection(XMUINT2 id2, XMINT2 offset);
	void CalcDests(XMUINT2 id2);
	bool GetCurID2(const Geometrics::Ray& ray, const Geometrics::PlaneInf& detectPlane, XMUINT2* id);
	enum TILE_STATE {
		TILE_STATE_NONE,
		TILE_STATE_TILE,
		TILE_STATE_P1,
		TILE_STATE_P2,
	};
	struct PlaySpace
	{
		XMFLOAT3 pos;
		TILE_STATE state;
		//maybe not in use
		//unsigned int tokenID, tileID;
		PlaySpace(XMFLOAT3 p, TILE_STATE state) :pos(p), state(state) {}
		PlaySpace() {}
	};
	PlaySpace** playSpace;

	enum PLAY_STATE
	{
		PLAY_STATE_P1_TOKEN,
		PLAY_STATE_P1_TILE,
		PLAY_STATE_P2_TOKEN,
		PLAY_STATE_P2_TILE
	}curPlayState;

	XMMATRIX tileSpaceMat;
	XMMATRIX invTileSpaceMat;
	Geometrics::PlaneInf tileDetectPlane;
	Geometrics::PlaneInf tokenDetectPlane;
	int holdingTokenObjID;
	int holdingSpaceID;
	int curIdx;
	int holdingTileObjID;
	std::unordered_set<unsigned int> tempDest;
	bool p1Turn;
};
