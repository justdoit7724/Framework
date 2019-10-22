#include "DebuggingScene.h"
#include "Debugging.h"

DebuggingScene::DebuggingScene()
	:Scene("Debugging")
{
}

void DebuggingScene::Update_Logic()
{
}

void DebuggingScene::Render_Update(const Camera* camera,float elapsed,float spf)
{
	Debugging::Instance()->Update(spf);
}

void DebuggingScene::Render() const
{
	Debugging::Instance()->Render();
}
