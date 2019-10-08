#include "DebuggingScene.h"
#include "Debugging.h"

DebuggingScene::DebuggingScene()
	:Scene("Debugging")
{
}

void DebuggingScene::Logic_Update()
{
}

void DebuggingScene::Render_Update(const Camera* camera,float elapsed)
{
	Debugging::Instance()->Update(camera);
}

void DebuggingScene::Render() const
{
	Debugging::Instance()->Render();
}
