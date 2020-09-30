
#include "pch.h"

#include "DebuggingScene.h"
#include "Debugging.h"

using namespace DX;

DebuggingScene::DebuggingScene()
{
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