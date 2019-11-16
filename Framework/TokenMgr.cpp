#include "TokenMgr.h"
#include "Object.h"
#include "MeshLoader.h"
#include "ShaderFormat.h"
#include "Camera.h"
#include "Transform.h"
#include "TextureMgr.h"
#include "Shader.h"
#include "DepthStencilState.h"
#include "BlendState.h"
#include "RasterizerState.h"
#include "Light.h"

//debug remove
#include "Cylinder.h"
std::vector<Object*> meshes;

TokenMgr::TokenMgr(const std::vector<XMFLOAT3>& firstArrange)
{
	TextureMgr::Instance()->Load("token", "Data\\Model\\Token\\pawn.png");
	TextureMgr::Instance()->Load("tokenNormal", "Data\\Model\\Token\\pawn_normal.png");
	//TextureMgr::Instance()->Load("foot", "Data\\Model\\Token\\foot.jpg");
	//TextureMgr::Instance()->Load("footNormal", "Data\\Model\\Token\\foot_normal.jpg");

	ID3D11ShaderResourceView* tokenSRV = TextureMgr::Instance()->Get("token");
	ID3D11ShaderResourceView* tokenNormalSRV = TextureMgr::Instance()->Get("tokenNormal");
	//ID3D11ShaderResourceView* footSRV = TextureMgr::Instance()->Get("foot");
	//ID3D11ShaderResourceView* footNormalSRV = TextureMgr::Instance()->Get("footNormal");
	
	MeshLoader::LoadToken(meshes);

	Shape* sharedShape = meshes[1]->shape;
	tokens.resize(firstArrange.size());
	for (int i = 0; i < firstArrange.size(); ++i)
	{
		if (i <= 2)
		{
			tokens[i] = new Object(sharedShape, tokenSRV, tokenNormalSRV);
		}
		else
		{
			tokens[i] = new Object(sharedShape, tokenSRV, tokenNormalSRV);
		}
		tokens[i]->transform->SetScale(0.02f, 0.02f, 0.02f);
		tokens[i]->transform->SetTranslation(firstArrange[i]);
	}
}

TokenMgr::~TokenMgr()
{
	for (auto token : tokens)
	{
		delete token;
	}
}

void TokenMgr::Move(UINT id, XMFLOAT3 to)
{
	tokens[id]->transform->SetTranslation(to);
}


void TokenMgr::Update()
{
	for (auto token : tokens)
	{
		token->Update();
	}
}

#include "CameraMgr.h"
void TokenMgr::Render(const XMMATRIX& vp, XMFLOAT3 eye, UINT sceneDepth)const
{
	DirectionalLight::Apply();
	PointLight::Apply();
	SpotLight::Apply();

	for (auto token : tokens)
	{
		const SHADER_STD_TRANSF STransformation(token->transform->WorldMatrix(), vp, XMMatrixIdentity());

		token->vs->WriteCB(0, (void*)(&STransformation));
		token->ps->WriteCB(3, &XMFLOAT4(eye.x, eye.y, eye.z, 0));

		token->vs->Apply();
		token->hs->Apply();
		token->ds->Apply();
		token->gs->Apply();
		token->ps->Apply();

		token->dsState->Apply();
		token->blendState->Apply();
		token->rsState->Apply();

		token->shape->Apply();
	}
}
