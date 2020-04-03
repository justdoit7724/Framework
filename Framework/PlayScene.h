#pragma once
#include "Scene.h"
#include "ObserverDP.h"

class UIButton;
class UICanvas;
class DirectionalLight;
class Buffer;
class Tictactoe;
enum SpaceKind;

struct SpaceInfo
{
	int x, y;
	UIButton* btn;

	SpaceInfo() {}
	SpaceInfo(int x, int y, UIButton* btn)
		:x(x), y(y), btn(btn) {}
};

class PlayScene : public Scene, Observer
{
public:
	PlayScene();
	void Update(float elapsed, float spf) override;

private:
	
	Tictactoe* rule;

	DirectionalLight* dLight;
	Buffer* cbEye;

	UICanvas* canvas;
	SpaceInfo board[3][3];
	ID3D11ShaderResourceView* oMoveSRV;
	ID3D11ShaderResourceView* xMoveSRV;
	ID3D11ShaderResourceView* noneMoveSRV;

	void Notify(int id, const void* data) override;
};
