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
	Debugging::Instance()->Render();
}