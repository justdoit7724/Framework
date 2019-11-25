#pragma once
#ifndef _PlaySpace
#define _PlaySpace
#include "Geometrics.h"
#include "Token.h"
#include "Tile.h"

class PlaySpace
{
public :
	PlaySpace(XMFLOAT3 p);
	PlaySpace():pos(XMFLOAT3(0,0,0)),tile(nullptr), token(nullptr) {}
	const XMFLOAT3 pos;

	void SetToken(Token* newToken);
	void SetTile(Tile* newTile);
	Tile* GetTile() { return tile; }
	Token* GetToken() { return token; }

private:
	Tile* tile;
	Token* token;
};

#endif
