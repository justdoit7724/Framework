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
	TextureMgr::Instance()->Load("DefaultSRV2", "Data\\Texture\\sample2.jpg");
	TextureMgr::Instance()->Load("DefaultSRV3", "Data\\Texture\\sample3.jpg");
	ID3D11ShaderResourceView* defaultSRV = TextureMgr::Instance()->Get("DefaultSRV");
	ID3D11ShaderResourceView* defaultSRV2 = TextureMgr::Instance()->Get("DefaultSRV2");
	ID3D11ShaderResourceView* defaultSRV3 = TextureMgr::Instance()->Get("DefaultSRV3");

	canvas = new UICanvas(SCREEN_WIDTH, SCREEN_HEIGHT);
	UIButton* button = canvas->AddButton("ButtonTest", XMFLOAT2(0, 0), XMFLOAT2(380, 300), defaultSRV, defaultSRV2, defaultSRV3);
	Debugging::Instance()->Visualize(button);
	Debugging::Instance()->Visualize(canvas);
}
void Lobby::Update(float elapsed, float spf)
{
	canvas->Update(spf);

}

void Lobby::Render(const Camera* camera, UINT sceneDepth) const
{
	canvas->Render();
}
