#include "TileMgr.h"
#include "Cylinder.h"
#include "Object.h"
#include "Camera.h"
#include "Transform.h"
#include "Shader.h"
#include "DepthStencilState.h"
#include "BlendState.h"
#include "RasterizerState.h"
#include "ShaderFormat.h"
#include "CommObjInfo.h"

TileMgr::TileMgr()
{
	curTileID = OBJ_PICKING_NONE;
}

TileMgr::~TileMgr()
{
	for (int i = 0; i < tiles.size(); ++i)
		delete tiles[i];

	tiles.clear();
}

void TileMgr::Move(unsigned int id, XMFLOAT3 to)
{
	tiles[id]->transform->SetTranslation(to);
}

void TileMgr::Update(const Geometrics::Ray ray)
{
	for (auto t : tiles)
	{
		t->Update();
	}

	curTileID = OBJ_PICKING_NONE;
	for (int i = 0; i < tiles.size(); ++i)
	{
		if (tiles[i]->IsPicking(ray))
		{
			curTileID = i;
			break;
		}
	}
}

void TileMgr::Render(const XMMATRIX& vp, XMFLOAT3 eye, const Frustum* frustum, unsigned int sceneDepth)
{
	if (sceneDepth >= 1)
	{
		for (int i = 0; i < tiles.size(); ++i)
		{
			if (tiles[i]->IsInsideFrustum(frustum))
			{
				Render(i, vp, eye);
			}
		}
	}
	else
		for (int i = 0; i < tiles.size(); ++i)
		{
			Render(i, vp, eye);
		}

}

void TileMgr::Notify(unsigned int id, const void* data)
{
	switch (id)
	{
	case OBS_TILE_FIRST_ARRANGE:

		const XMFLOAT3* pos = (const XMFLOAT3*)data;


		tiles.resize(TILE_OBJ_COUNT);

		for (int i = 0; i < TILE_OBJ_COUNT; ++i)
		{
			tiles[i] = new Object(new Cylinder(40), nullptr, nullptr);
			tiles[i]->transform->SetScale(10, 1, 10);
			tiles[i]->transform->SetTranslation(pos[i]);
		}
		break;
	}
}

void TileMgr::Render(unsigned int id, const XMMATRIX& vp, XMFLOAT3 eye)
{
	const SHADER_STD_TRANSF STransformation(tiles[id]->transform->WorldMatrix(), vp, XMMatrixIdentity());

	tiles[id]->vs->WriteCB(0, (void*)(&STransformation));
	tiles[id]->ps->WriteCB(3, &XMFLOAT4(eye.x, eye.y, eye.z, 0));

	tiles[id]->vs->Apply();
	tiles[id]->hs->Apply();
	tiles[id]->ds->Apply();
	tiles[id]->gs->Apply();
	tiles[id]->ps->Apply();

	tiles[id]->dsState->Apply();
	tiles[id]->blendState->Apply();
	tiles[id]->rsState->Apply();
		
	tiles[id]->shape->Apply();
}
