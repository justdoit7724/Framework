﻿#pragma once
#include "Geometrics.h"
#include <vector>

class NonagaLogic
{
public:
	NonagaLogic(
		std::vector<XMFLOAT3>* firstTileArrange,
		std::vector<XMFLOAT3>* firstTokenArrange);
	~NonagaLogic();
	
	void MoveToken(unsigned int* id, XMFLOAT3* pos);
	void MoveTile(unsigned int* id, XMFLOAT3* pos);
	bool PreviewToken(bool* isPossible, XMFLOAT3* pos);
	bool PreviewTile(bool* isPossible, XMFLOAT3* pos);
private:
	NonagaLogic() {}
	enum {
		TILE_STATE_P1 = 1,
		TILE_STATE_P2 = 2,
		TILE_STATE_TILE = 4
	};
	struct PlaySpace
	{
		XMFLOAT3 pos;
		unsigned int state;
		unsigned int tokenID, tileID;
		PlaySpace(XMFLOAT3 p) :pos(p), state(0), tokenID(-1), tileID(-1) {}
	private:
		PlaySpace() {}
	};
	PlaySpace** playSpace;
};

