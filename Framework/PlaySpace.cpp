#include "PlaySpace.h"

PlaySpace::PlaySpace(XMFLOAT3 p)
	:pos(p),tile(nullptr),token(nullptr) {

	
}

void PlaySpace::SetToken(Token* newToken)
{
	token = newToken;
}

void PlaySpace::SetTile(Tile* newTile)
{
	tile = newTile;
}
