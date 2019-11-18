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
	
	void MoveToken(unsigned int* id, XMFLOAT3* pos);
	void MoveTile(unsigned int* id, XMFLOAT3* pos);
	bool PreviewToken(bool* isPossible, XMFLOAT3* pos);
	bool PreviewTile(bool* isPossible, XMFLOAT3* pos);
private:
	void CheckDirection(XMUINT2 id2, XMINT2 offset);
	void CalcDests(XMUINT2 id2);
	bool GetCurID2(const Geometrics::Ray ray, XMUINT2* id);
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
	unsigned int holdingToken;
	unsigned int curIdx;
	unsigned int holdingTile;
	std::unordered_set<unsigned int> tempDest;
};
