#include "Tile.h"
#include "PlaySpace.h"
#include "Object.h"
#include "Transform.h"
#include "TextureMgr.h"

#include "Cylinder.h"

Tile::Tile(PlaySpace* const* space, unsigned int id)
	:space(space), id(id)
{
	TextureMgr::Instance()->Load("normal", "Data\\Texture\\default_normal.png");
	TextureMgr::Instance()->Load("red", "Data\\Texture\\red_light.png");
	TextureMgr::Instance()->Load("green", "Data\\Texture\\green_light.png");
	obj = new Object(new Cylinder(30), TextureMgr::Instance()->Get("green"), TextureMgr::Instance()->Get("normal"));
	obj->transform->SetScale(10, 1, 10);
}

Tile::Tile(bool isRed)
{
	TextureMgr::Instance()->Load("normal", "Data\\Texture\\default_normal.png");
	TextureMgr::Instance()->Load("red", "Data\\Texture\\red_light.png");
	TextureMgr::Instance()->Load("green", "Data\\Texture\\green_light.png");
	obj = new Object(new Cylinder(30), TextureMgr::Instance()->Get(isRed ? "red" : "green"), TextureMgr::Instance()->Get("normal"));
	obj->transform->SetScale(10, 1, 10);
}

Tile::~Tile()
{
	delete obj;
}


void Tile::Move(unsigned int toID)
{
	space[id]->state = TILE_STATE_NONE;
	id = toID;
	
	obj->transform->SetTranslation(space[toID]->pos);
	space[toID]->state = TILE_STATE_TILE;
}

void Tile::RawMove(XMFLOAT3 pos)
{
	obj->transform->SetTranslation(pos);
}

void Tile::UpdateBound()
{
	obj->UpdateBound();
}

void Tile::Render(const XMMATRIX& vp, XMFLOAT3 eye, unsigned int sceneDepth) const
{
	obj->Render(vp, eye, sceneDepth);

}

bool Tile::IsPicking(const Geometrics::Ray ray)
{
	return obj->IsPicking(ray);
}

void Tile::SetEnabled(bool e)
{
	obj->SetEnabled(e);
}

Object* Tile::Obj()
{
	return obj;
}
