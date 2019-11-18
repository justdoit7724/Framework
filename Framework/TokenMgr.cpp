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
#include "Debugging.h"
#include "CommObjInfo.h"
#include "TextureMgr.h"


//debug remove
#include "Cylinder.h"
std::vector<Object*> meshes;

TokenMgr::TokenMgr()
{
	curPickingTokenID = OBJ_PICKING_NONE;

	TextureMgr::Instance()->Load("Red", "Data\\Texture\\red_light.png");
	TextureMgr::Instance()->Load("Green", "Data\\Texture\\green_light.png");
	redIndicator = new Object(new Cylinder(5), TextureMgr::Instance()->Get("Red"), nullptr);
	redIndicator->SetEnabled(false);
	redIndicator->transform->SetScale(10, 10, 10);
	greenIndicator = new Object(new Cylinder(5), TextureMgr::Instance()->Get("Green"), nullptr);
	greenIndicator->SetEnabled(false);
	greenIndicator->transform->SetScale(10, 10, 10);
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


void TokenMgr::Update(const Geometrics::Ray ray)
{
	for (int i=0; i< tokens.size(); ++i)
	{
		tokens[i]->Update();
	}

	curPickingTokenID = OBJ_PICKING_NONE;
	for (int i = 0; i < tokens.size(); ++i)
	{
		if (tokens[i]->IsPicking(ray))
		{
			curPickingTokenID = i;
			break;
		}
	}
}

void TokenMgr::Render(const XMMATRIX& vp, XMFLOAT3 eye, const Frustum* frustum, UINT sceneDepth)const
{
	DirectionalLight::Apply();
	PointLight::Apply();
	SpotLight::Apply();

	for (auto token : tokens)
	{
		token->Render(vp, eye, sceneDepth);
	}
	redIndicator->Render(vp, eye, sceneDepth);
	greenIndicator->Render(vp, eye, sceneDepth);
}

void TokenMgr::Notify(unsigned int id, const void* data)
{
	switch (id)
	{
	case OBS_TOKEN_FIRST_ARRANGE:
	{
		TextureMgr::Instance()->Load("token", "Data\\Model\\Token\\pawn.png");
		TextureMgr::Instance()->Load("tokenNormal", "Data\\Model\\Token\\pawn_normal.png");
		//TextureMgr::Instance()->Load("foot", "Data\\Model\\Token\\foot.jpg");
		//TextureMgr::Instance()->Load("footNormal", "Data\\Model\\Token\\foot_normal.jpg");

		ID3D11ShaderResourceView* tokenSRV = TextureMgr::Instance()->Get("token");
		ID3D11ShaderResourceView* tokenNormalSRV = TextureMgr::Instance()->Get("tokenNormal");
		//ID3D11ShaderResourceView* footSRV = TextureMgr::Instance()->Get("foot");
		//ID3D11ShaderResourceView* footNormalSRV = TextureMgr::Instance()->Get("footNormal");

		const XMFLOAT3* pos = (const XMFLOAT3*)data;
		MeshLoader::LoadToken(meshes);
		Shape* sharedShape = meshes[1]->shape;
		tokens.resize(TOKEN_OBJ_COUNT_TOTAL);
		for (int i = 0; i < TOKEN_OBJ_COUNT_TOTAL; ++i)
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
			tokens[i]->transform->SetTranslation(pos[i]);

			Debugging::Instance()->Visualize(tokens[i]);
		}
	}
		break;
	case OBS_TOKEN_ENABLE_INDICATOR_RED:
	{
		const XMFLOAT3* pos = (const XMFLOAT3*)data;
		redIndicator->transform->SetTranslation(*pos);
		redIndicator->SetEnabled(true);
		greenIndicator->SetEnabled(false);
	}
		break;
	case OBS_TOKEN_ENABLE_INDICATOR_GREEN:
	{
		const XMFLOAT3* pos = (const XMFLOAT3*)data;
		greenIndicator->transform->SetTranslation(*pos);
		greenIndicator->SetEnabled(true);
		redIndicator->SetEnabled(false);
	}
		break;
	case OBS_TOKEN_DISABLE_INDICATOR:
		redIndicator->SetEnabled(false);
		greenIndicator->SetEnabled(false);
		break;
	case OBS_TOKEN_MOVE:
		const XMFLOAT4* mData = (const XMFLOAT4*)data;
		XMFLOAT3 pos = XMFLOAT3(
			mData->x,
			mData->y,
			mData->z);
		UINT tokenID = mData->w;

		tokens[tokenID]->transform->SetTranslation(pos);
		greenIndicator->SetEnabled(false);

		break;
	}
}
