#include "Token.h"
#include "Object.h"
#include "Transform.h"
#include "Cube.h"
#include "TextureMgr.h"

Token::Token(unsigned int id, bool p1)
	:id(id), isP1(p1)
{
	TextureMgr::Instance()->Load("token", "Data\\Model\\Token\\pawn.png");
	TextureMgr::Instance()->Load("tokenNormal", "Data\\Model\\Token\\pawn_normal.png");
	//TextureMgr::Instance()->Load("foot", "Data\\Model\\Token\\foot.jpg");
	//TextureMgr::Instance()->Load("footNormal", "Data\\Model\\Token\\foot_normal.jpg");


	obj = new Object(new Cube(), TextureMgr::Instance()->Get("token"), TextureMgr::Instance()->Get("tokenNormal"));
	obj->transform->SetScale(10, 20, 10);
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

void Token::Move(int toId, XMFLOAT3 pos)
{
	id = toId;
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


