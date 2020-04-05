#pragma once
#include "Scene.h"

class DirectionalLight;
class Buffer;
class Tictactoe;
class TTTRender;
class TTTAI;



class PlayScene : public Scene
{
public:
	PlayScene();
	void Update(float elapsed, float spf) override;

private:
	
	Tictactoe* rule;
	TTTRender* render;
	TTTAI* ai;

	DirectionalLight* dLight;
	Buffer* cbEye;
};
