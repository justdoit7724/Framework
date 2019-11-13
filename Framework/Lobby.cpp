#include "Lobby.h"
#include "UI.h"
#include "Camera.h"
#include "TextureMgr.h"
#include "Keyboard.h"

#include "Debugging.h"

Lobby::Lobby()
	:Scene("Lobby")
{
	Debugging::Instance()->EnableGrid(10);

	TextureMgr::Instance()->Load("DefaultSRV", "Data\\Texture\\sample.jpg");
	ID3D11ShaderResourceView* defaultSRV = TextureMgr::Instance()->Get("DefaultSRV");

	canvas = new UICanvas(SCREEN_WIDTH, SCREEN_HEIGHT);
	canvas->Add("TestUI", XMFLOAT2(0, 0), 300, 300, 0, defaultSRV);
}
void Lobby::Update(float elapsed, float spf)
{
	canvas->Update(spf);

}

void Lobby::Render(const Camera* camera, UINT sceneDepth) const
{
	canvas->Render();
}
