#include "DebuggingScene.h"
#include "Debugging.h"

DebuggingScene::DebuggingScene()
{
	Debugging::Instance();
}

DebuggingScene::~DebuggingScene()
{
	Debugging::Instance()->Release();
}

void DebuggingScene::Update(float elapsed,float spf)
{
	Debugging::Instance()->Update(spf);
}

void DebuggingScene::Render(const Camera* camera, UINT sceneDepth) const
{
	Debugging::Instance()->Render(camera);
}
