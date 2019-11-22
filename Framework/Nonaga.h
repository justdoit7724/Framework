#pragma once
#include "Geometrics.h"
#include "TileSpaceInfo.h"
#include <vector>

class Token;
class Tile;
class PlaySpace;
class Object;

#define NONE -1

class NonagaLogic
{
public:
	NonagaLogic(PlaySpace* const* space);
	~NonagaLogic();

	int CanMoveToken(int from, int to);
	bool CanPickTile(int id);
	bool CanMoveTileTo(int from, int to);
	void TokenMove(int from, int to);
	void TileMove(int from, int to);
	int GetScore(const std::vector<Token*>& tokens);
	bool IsP1Turn() { return p1Turn; }

private:
	PlaySpace* const* space;
	int unmovableTileID;
	bool p1Turn;
	bool CheckDirection(XMINT2 holdingTokenID2, XMINT2 offset, XMINT2& destId2);
	int CheckNeighbor(int centerID, int except);
};

class NonagaStage
{
public:
	NonagaStage();
	~NonagaStage();

	void Update(const Geometrics::Ray ray);
	void Objs(std::vector<Object*>& objOutput);

	void Render(const XMMATRIX& vp, XMFLOAT3 eye, unsigned int sceneDepth)const;
	
private:
	bool IsWin();
	void TokenDragStart(const Geometrics::Ray ray);
	void TokenDragging();
	void TileDragStart(const Geometrics::Ray ray);
	void TileDragging();
	bool GetCurID2(const Geometrics::Ray& ray, const Geometrics::PlaneInf& detectPlane);

	
	PlaySpace* playSpace[TILE_SPACE_COUNT_Z * TILE_SPACE_COUNT_X]{ nullptr };

	NonagaLogic* logic;

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
	int pDetectID;
	int holdingTokenID;
	int holdingTileID;
	
	bool isMove;

	// 0~2 = p1
	// 3~5 = p2
	std::vector<Token*> tokens;
	Token* redToken, * greenToken;
	std::vector < Tile*> tiles;
	Tile* redTile, *greenTile;
};

