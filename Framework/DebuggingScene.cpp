#include "DebuggingScene.h"
#include "Debugging.h"

DebuggingScene::DebuggingScene()
	:Scene("Debugging")
{
}

void DebuggingScene::Logic_Update(Camera* camera)
{
}

void DebuggingScene::Render_Update(Camera* camera)
{
	Debugging::Instance()->Update(camera);
}

void DebuggingScene::Render() const
{
	Debugging::Instance()->Render();
}
