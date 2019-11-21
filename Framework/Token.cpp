#include "Token.h"
#include "PlaySpace.h"
#include "Object.h"
#include "Transform.h"
#include "Cube.h"
#include "NonagaLogic.h"
#include "TextureMgr.h"
#include "TileSpaceInfo.h"

Token::Token(PlaySpace*const* space, unsigned int id, bool p1)
	:space(space), id(id), isP1(p1)
{
	TextureMgr::Instance()->Load("token", "Data\\Model\\Token\\pawn.png");
	TextureMgr::Instance()->Load("tokenNormal", "Data\\Model\\Token\\pawn_normal.png");
	//TextureMgr::Instance()->Load("foot", "Data\\Model\\Token\\foot.jpg");
	//TextureMgr::Instance()->Load("footNormal", "Data\\Model\\Token\\foot_normal.jpg");


	obj = new Object(new Cube(), TextureMgr::Instance()->Get("token"), TextureMgr::Instance()->Get("tokenNormal"));
	obj->transform->SetScale(10, 20, 10);
	obj->transform->SetTranslation(space[id]->pos);
	space[id]->state = isP1 ? TILE_STATE_P1 : TILE_STATE_P2;
}

Token::Token(bool isRed)
{
	TextureMgr::Instance()->Load("red", "Data\\Texture\\red_light.png");
	TextureMgr::Instance()->Load("green", "Data\\Texture\\green_light.png");
	obj = new Object(new Cube(), TextureMgr::Instance()->Get(isRed?"red":"green"), nullptr);
	obj->transform->SetScale(7, 10, 7);
	obj->SetEnabled(false);
}

Token::~Token()
{
	delete obj;
}

void Token::Move(unsigned int toID)
{
	assert(id != toID);
	obj->transform->SetTranslation(space[toID]->pos);
	space[toID]->state = isP1 ? TILE_STATE_P1 : TILE_STATE_P2;

	space[id]->state = TILE_STATE_TILE;

	id = toID;
}

void Token::RawMove(XMFLOAT3 pos)
{
	obj->transform->SetTranslation(pos);
}

bool Token::IsPicking(const Geometrics::Ray ray)
{
	return obj->IsPicking(ray);
}

void Token::UpdateBound()
{
	obj->UpdateBound();
}

void Token::Render(const XMMATRIX& vp, XMFLOAT3 eye, unsigned int sceneDepth) const
{
	obj->Render(vp, eye, sceneDepth);
}

void Token::SetEnabled(bool b)
{
	obj->SetEnabled(b);
}


