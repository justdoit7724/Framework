#include "DebuggingScene.h"
#include "Debugging.h"

DebuggingScene::DebuggingScene()
	:Scene("Debugging")
{
}

void DebuggingScene::Update(float elapsed,float spf)
{
	Debugging::Instance()->Update(spf);
}

void DebuggingScene::Render(const Camera* camera, UINT sceneDepth) const
{
	Debugging::Instance()->Render();
}
