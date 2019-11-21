#pragma once
#include "Geometrics.h"

class Token;
class Tile;

enum TILE_STATE {
	TILE_STATE_NONE,
	TILE_STATE_TILE,
	TILE_STATE_P1,
	TILE_STATE_P2,
};
class PlaySpace
{
public :
	PlaySpace(XMFLOAT3 p);
	PlaySpace():pos(XMFLOAT3(0,0,0)) {}
	TILE_STATE State() { return state; }
	const XMFLOAT3 pos;

private:
	friend class Token;
	friend class Tile;
	TILE_STATE state;
};
