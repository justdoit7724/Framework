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
#include "Cube.h"

std::vector<Object*> meshes;
TokenMgr::TokenMgr()
{
	tokens.resize(6);

	TextureMgr::Instance()->Load("Default1", "Data\\Texture\\sample.jpg");
	TextureMgr::Instance()->Load("Default2", "Data\\Texture\\sample2.jpg");
	TextureMgr::Instance()->Load("Default3", "Data\\Texture\\sample3.jpg");
	TextureMgr::Instance()->Load("DefaultNormal", "Data\\Texture\\default_normal.png");

	ID3D11ShaderResourceView* default1 = TextureMgr::Instance()->Get("Default1");
	ID3D11ShaderResourceView* default2 = TextureMgr::Instance()->Get("Default2");
	ID3D11ShaderResourceView* default3 = TextureMgr::Instance()->Get("Default3");
	ID3D11ShaderResourceView* defaultNormal = TextureMgr::Instance()->Get("DefaultNormal");
	
	tokens[0] = new Object(new Cube(), default1, defaultNormal);
	tokens[1] = new Object(new Cube(), default2, defaultNormal);
	tokens[2] = new Object(new Cube(), default3, defaultNormal);
	tokens[0]->transform->SetScale(10, 10, 10);
	tokens[1]->transform->SetScale(10, 10, 10);
	tokens[2]->transform->SetScale(10, 10, 10);

	tokens[3] = new Object(new Cube(), default1, defaultNormal);
	tokens[4] = new Object(new Cube(), default2, defaultNormal);
	tokens[5] = new Object(new Cube(), default3, defaultNormal);
	tokens[3]->transform->SetScale(10, 10, 10);
	tokens[4]->transform->SetScale(10, 10, 10);
	tokens[5]->transform->SetScale(10, 10, 10);

	//MeshLoader::Load(meshes, "Token", "Checkboard.obj");
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
void TokenMgr::Render(const XMMATRIX& vp, XMFLOAT3 eye)const
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
