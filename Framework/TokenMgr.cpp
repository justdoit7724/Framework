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
TokenMgr::TokenMgr()
{
	tokens.resize(6);

	TextureMgr::Instance()->Load("token", "Data\\Model\\Token\\pawn.png");
	TextureMgr::Instance()->Load("tokenNormal", "Data\\Model\\Token\\pawn_normal.png");
	//TextureMgr::Instance()->Load("foot", "Data\\Model\\Token\\foot.jpg");
	//TextureMgr::Instance()->Load("footNormal", "Data\\Model\\Token\\foot_normal.jpg");

	ID3D11ShaderResourceView* tOpaqueSRV = TextureMgr::Instance()->Get("token");
	ID3D11ShaderResourceView* tOpaqueNormalSRV = TextureMgr::Instance()->Get("tokenNormal");
	//ID3D11ShaderResourceView* footSRV = TextureMgr::Instance()->Get("foot");
	//ID3D11ShaderResourceView* footNormalSRV = TextureMgr::Instance()->Get("footNormal");
	
	MeshLoader::LoadToken(meshes);
	tokens[0] = new Object(meshes[1]->shape, tOpaqueSRV, tOpaqueNormalSRV);
	tokens[1] = new Object(meshes[1]->shape, tOpaqueSRV, tOpaqueNormalSRV);
	tokens[2] = new Object(meshes[1]->shape, tOpaqueSRV, tOpaqueNormalSRV);
	tokens[0]->transform->SetTranslation(10, 0, 10);
	tokens[1]->transform->SetTranslation(30, 0, 10);
	tokens[2]->transform->SetTranslation(10, 0, 30);
	tokens[0]->transform->SetScale(0.02f, 0.02f, 0.02f);
	tokens[1]->transform->SetScale(0.02f, 0.02f, 0.02f);
	tokens[2]->transform->SetScale(0.02f, 0.02f, 0.02f);

	tokens[3] = new Object(meshes[1]->shape, tOpaqueSRV, tOpaqueNormalSRV);
	tokens[4] = new Object(meshes[1]->shape, tOpaqueSRV, tOpaqueNormalSRV);
	tokens[5] = new Object(meshes[1]->shape, tOpaqueSRV, tOpaqueNormalSRV);
	tokens[3]->transform->SetTranslation(-10, 0,-10);
	tokens[4]->transform->SetTranslation(-30, 0, -10);
	tokens[5]->transform->SetTranslation(-10, 0, -30);
	tokens[3]->transform->SetScale(0.02f, 0.02f, 0.02f);
	tokens[4]->transform->SetScale(0.02f, 0.02f, 0.02f);
	tokens[5]->transform->SetScale(0.02f, 0.02f, 0.02f);


}

TokenMgr::~TokenMgr()
{
	for (auto token : tokens)
	{
		delete token;
	}
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
