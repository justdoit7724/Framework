#include "Tile.h"
#include "Object.h"
#include "Transform.h"
#include "TextureMgr.h"
#include "Shader.h"
#include "ShaderReg.h"
#include "Cylinder.h"

Tile::Tile(unsigned int id)
	:Object(new Cylinder(30), nullptr, nullptr),id(id)
{
	TextureMgr::Instance()->Load("normal", "Data\\Texture\\default_normal.png");
	TextureMgr::Instance()->Load("red", "Data\\Texture\\red_light.png");
	TextureMgr::Instance()->Load("green", "Data\\Texture\\green_light.png");

	ps->WriteSRV(SHADER_REG_PS_SRV_DIFFUSE, TextureMgr::Instance()->Get("green")); 
	ps->WriteSRV(SHADER_REG_PS_SRV_NORMAL, TextureMgr::Instance()->Get("normal"));
	
	transform->SetScale(10, 1, 10);
}

Tile::Tile(bool isRed)
	:Object(new Cylinder(30), nullptr, nullptr)
{
	TextureMgr::Instance()->Load("normal", "Data\\Texture\\default_normal.png");
	TextureMgr::Instance()->Load("red", "Data\\Texture\\red_light.png");
	TextureMgr::Instance()->Load("green", "Data\\Texture\\green_light.png");

	ps->WriteSRV(SHADER_REG_PS_SRV_DIFFUSE, TextureMgr::Instance()->Get(isRed ? "red" : "green"));
	ps->WriteSRV(SHADER_REG_PS_SRV_NORMAL, TextureMgr::Instance()->Get("normal"));

	transform->SetScale(10, 1, 10);
}

void Tile::Move(int toId, XMFLOAT3 pos)
{
	id = toId;
	transform->SetTranslation(pos);
}
