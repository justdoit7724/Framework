#include "DebuggingScene.h"
#include "Debugging.h"

DebuggingScene::DebuggingScene()
	:Scene("Debugging")
{
}

void DebuggingScene::Logic_Update()
{
}

void DebuggingScene::Render_Update(const Camera* camera,float elapsed,float spf)
{
	Debugging::Instance()->Update(camera, spf);
}

void DebuggingScene::Render() const
{
	Debugging::Instance()->Render();
}
