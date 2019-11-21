#pragma once
#include "Geometrics.h"
#include "TileSpaceInfo.h"

class Token;
class Tile;
class PlaySpace;
class Object;

#define OBJ_PICKING_NONE -1

class NonagaLogic
{
public:
	NonagaLogic();
	~NonagaLogic();

	void Update(const Geometrics::Ray ray);
	void Render(const XMMATRIX& vp, XMFLOAT3 eye, unsigned int sceneDepth);

	
private:
	int SingleID();
	int GetScore();
	bool IsWin();
	void TokenDragStart(const Geometrics::Ray ray);
	void TokenDragging();
	void TokenMove();
	void TileDragStart(const Geometrics::Ray ray);
	void TileDragging();
	void TileMove();
	bool CanMove();
	bool CanMoveTile(unsigned int toID);
	bool GetCurID2(const Geometrics::Ray& ray, const Geometrics::PlaneInf& detectPlane);

	
	PlaySpace* playSpace[TILE_SPACE_COUNT_Z * TILE_SPACE_COUNT_X]{ nullptr };

	enum PLAY_STATE
	{
		PLAY_STATE_P1_TOKEN,
		PLAY_STATE_P1_TILE,
		PLAY_STATE_P2_TOKEN,
		PLAY_STATE_P2_TILE,
		PLAY_STATE_FINISH
	}curPlayState;

	XMMATRIX tileSpaceMat;
	XMMATRIX invTileSpaceMat;
	Geometrics::PlaneInf detectPlane;
	Token* holdingToken;
	Tile* holdingTile;
	XMINT2 pDetectID2;
	
	int unmovableTileID;
	bool p1Turn;
	bool isMove;

	// 0~2 = p1
	// 3~5 = p2
	Token* tokens[TOKEN_OBJ_COUNT_TOTAL];
	Token* redToken, * greenToken;
	Tile* tiles[TILE_OBJ_COUNT];
	Tile* redTile, *greenTile;
};
