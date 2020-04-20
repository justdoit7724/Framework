#include "stdafx.h"
#include "PlayScene.h"
#include "Shader.h"
#include "ShaderReg.h"
#include "Game_info.h"
#include "Light.h"
#include "Buffer.h"
#include "Keyboard.h"
#include "Debugging.h"
#include "CustomSTL.h"
#include "AStar.h"
#include "ObjectPool.h"
#include "UI.h"
#include "TextureMgr.h"
#include "DepthStencilState.h"
#include "Timer.h"

PlayScene::PlayScene()
{
	TextureMgr::Instance()->Load("red", "Data\\Texture\\red_light.png");
	TextureMgr::Instance()->Load("green", "Data\\Texture\\green_light.png");
	TextureMgr::Instance()->Load("blue", "Data\\Texture\\blue_light.png");
	TextureMgr::Instance()->Load("white", "Data\\Texture\\white.png");
	TextureMgr::Instance()->Load("sample", "Data\\Texture\\sample.jpg");

	canvas = new UICanvas();

	mask = new UI(XMFLOAT2(200, 200), XMFLOAT2(300, 300), 0, nullptr);
	uiBackground = new UI(XMFLOAT2(0, 0), XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT), 1.0f, TextureMgr::Instance()->Get("white"));
	mainImage = new UI(XMFLOAT2(100, 100), XMFLOAT2(500, 400), 0.1f, TextureMgr::Instance()->Get("sample"));
	moveModeBtn = new UIButton(XMFLOAT2(20, 20), XMFLOAT2(40, 40), TextureMgr::Instance()->Get("blue"));
	//shapeCircleModeBtn = new UIButton(XMFLOAT2())

	D3D11_DEPTH_STENCIL_DESC maskDesc;
	ZeroMemory(&maskDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	maskDesc.DepthEnable = true;
	maskDesc.DepthFunc = D3D11_COMPARISON_LESS;
	maskDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	maskDesc.StencilEnable = true;
	maskDesc.StencilReadMask = 0xff;
	maskDesc.StencilWriteMask = 0xff;
	maskDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	maskDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	maskDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	maskDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	maskDesc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;
	maskDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	maskDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	maskDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	mask->dsState->Modify(&maskDesc);
	mask->dsState->SetRefValue(1);
	D3D11_DEPTH_STENCIL_DESC bkgDesc;
	bkgDesc.DepthEnable = true;
	bkgDesc.DepthFunc = D3D11_COMPARISON_LESS;
	bkgDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	bkgDesc.StencilEnable = true;
	bkgDesc.StencilReadMask = 0xff;
	bkgDesc.StencilWriteMask = 0xff;
	bkgDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	bkgDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	bkgDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	bkgDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	bkgDesc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;
	bkgDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	bkgDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	bkgDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	uiBackground->dsState->Modify(&bkgDesc);
	D3D11_DEPTH_STENCIL_DESC mainImageDesc;
	mainImageDesc.DepthEnable = true;
	mainImageDesc.DepthFunc = D3D11_COMPARISON_LESS;
	mainImageDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	mainImageDesc.StencilEnable = true;
	mainImageDesc.StencilReadMask = 0xff;
	mainImageDesc.StencilWriteMask = 0xff;
	mainImageDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	mainImageDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	mainImageDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	mainImageDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	mainImageDesc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;
	mainImageDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	mainImageDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	mainImageDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	mainImage->dsState->Modify(&mainImageDesc);
	mainImage->dsState->SetRefValue(1);


	dLight = new DirectionalLight(
		XMFLOAT3(0.4, 0.4, 0.4),
		XMFLOAT3(0.7, 0.7, 0.7),
		XMFLOAT3(0.9, 0.9, 0.9),
		Normalize(XMFLOAT3(1, -0.5, 1)));

	cbEye = new Buffer(sizeof(XMFLOAT4));
}


void PlayScene::Update(float elapsed, float spf)
{
	moveModeBtn->Update(canvas);
}

void PlayScene::Notify(int id, const void* data)
{
	
}
