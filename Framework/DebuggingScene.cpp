#include "stdafx.h"
#include "DebuggingScene.h"
#include "Debugging.h"

DebuggingScene::DebuggingScene()
{
	Debugging::Instance()->EnableGrid(10);
}

DebuggingScene::~DebuggingScene()
{
	Debugging::Instance()->Release();
}

void DebuggingScene::Update(float elapsed, float spf)
{
	Debugging::Instance()->Update(spf);
}

void DebuggingScene::Render(const XMMATRIX& vp, const Frustum& frustum, UINT sceneDepth, void const* subject) const
{
	Debugging::Instance()->Render(vp);
}