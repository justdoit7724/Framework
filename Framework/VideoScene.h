#pragma once
#include "Scene.h"

class Timer;

class VideoScene :
	public Scene
{
public:
	VideoScene();


	void Logic_Update()override;
	void Render_Update(const Camera* camera, float elapsed, float spf)override;
	void Render()const override;

private:
	Timer* timer;

	Camera* sceneCam;

	Object* red;
	Object* green;
	Object* green2;
	Object* blue;

	XMMATRIX redMat;
	XMMATRIX greenMat;
	XMMATRIX blueMat;

	float totalTime;
	float curTime;
};

