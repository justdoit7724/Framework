#include "Token.h"
#include "Transform.h"
#include "Cube.h"
#include "TextureMgr.h"
#include "ShaderFormat.h"
#include "Shader.h"
#include "Camera.h"
#include "ShaderReg.h"
#include "CameraMgr.h"

Token::Token(unsigned int id, bool p1)
	:Object(new Cube(),
		"StdDisplacementVS.cso", Std_ILayouts, ARRAYSIZE(Std_ILayouts),
		"StdDisplacementHS.cso", "StdDisplacementDS.cso", "",
		"StandardPS.cso", Z_ORDER_STANDARD),
		id(id), isP1(p1)
{
	TextureMgr::Instance()->Load("token", "Data\\Model\\Token\\pawn.png");
	TextureMgr::Instance()->Load("tokenNormal", "Data\\Model\\Token\\pawn_normal.png");
	TextureMgr::Instance()->Load("tokenDP", "Data\\Model\\Token\\pawn_displace.png");
	
	shape->SetPrimitiveType(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	vs->AddCB(0, 1, sizeof(SHADER_STD_TRANSF));
	vs->AddCB(1, 1, sizeof(XMFLOAT4));
	ds->AddCB(0, 1, sizeof(XMMATRIX));
	ds->AddCB(1, 1, sizeof(float));
	float dpScale = 2.0f;
	ds->WriteCB(1, &dpScale);
	ds->AddSRV(0, 1);
	ds->WriteSRV(0, TextureMgr::Instance()->Get("tokenDP"));
	D3D11_SAMPLER_DESC samp_desc;
	ZeroMemory(&samp_desc, sizeof(D3D11_SAMPLER_DESC));
	samp_desc.Filter = D3D11_FILTER_ANISOTROPIC;
	samp_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samp_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samp_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samp_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samp_desc.MinLOD = 0;
	samp_desc.MaxLOD = D3D11_FLOAT32_MAX;
	ds->AddSamp(0, 1, &samp_desc);
	ps->AddCB(SHADER_REG_PS_CB_MATERIAL, 1, sizeof(SHADER_MATERIAL));
	ps->WriteCB(SHADER_REG_PS_CB_MATERIAL, &SHADER_MATERIAL(XMFLOAT3(0.7, 0.7, 0.7), 1, XMFLOAT3(0.5,0.5,0.5), XMFLOAT3(0.8,0.8,0.8), 16));
	ps->AddSRV(SHADER_REG_PS_SRV_DIFFUSE, 1);
	ps->AddSRV(SHADER_REG_PS_SRV_NORMAL, 1);
	ps->WriteSRV(SHADER_REG_PS_SRV_DIFFUSE, TextureMgr::Instance()->Get("token"));
	ps->WriteSRV(SHADER_REG_PS_SRV_NORMAL, TextureMgr::Instance()->Get("tokenNormal"));
	ps->AddSamp(SHADER_REG_PS_SAMP_TEX, 1, &samp_desc);

	transform->SetScale(7, 15, 7);
}


Token::Token(bool isRed)
	:Object(new Cube(),
		nullptr,nullptr)
{
	TextureMgr::Instance()->Load("red", "Data\\Texture\\red_light.png");
	TextureMgr::Instance()->Load("green", "Data\\Texture\\green_light.png");
	ps->WriteSRV(SHADER_REG_PS_SRV_DIFFUSE, TextureMgr::Instance()->Get(isRed ? "red":"green"));
	ps->WriteSRV(SHADER_REG_PS_SRV_DIFFUSE, TextureMgr::Instance()->Get("tokenNormal"));

	transform->SetScale(7, 10, 7);
	enabled = false;
	ps->WriteSRV(SHADER_REG_PS_SRV_DIFFUSE, TextureMgr::Instance()->Get("token"));
	ps->WriteSRV(SHADER_REG_PS_SRV_NORMAL, TextureMgr::Instance()->Get("tokenNormal"));
}


void Token::Render(const XMMATRIX& parentWorld, const XMMATRIX& vp, UINT sceneDepth) const
{
	const SHADER_STD_TRANSF STransformation(transform->WorldMatrix()*parentWorld, vp);

	// USE main camera !!!
		vs->WriteCB(0, &STransformation);
	if(vs->CheckCBSlot(1))
		vs->WriteCB(1, &CameraMgr::Instance()->Main()->transform->GetPos());
	if(ds->CheckCBSlot(0))
		ds->WriteCB(0, &(STransformation.vp));

	Object::Render();
}

void Token::Move(int toId, XMFLOAT3 pos)
{
	id = toId;
	transform->SetTranslation(pos);
}


