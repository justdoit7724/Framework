#pragma once

#include "Scene.h"

class DirectionalLight;
class Buffer;
class Camera;
class Object;
class UICanvas;

class PlayScene : public Scene
{
public:
	PlayScene();

	virtual void Update(float elapsed, float spf) override;

	void CreateModel(std::string filename);
	void CreateGrid();

	void Lerp2CamX();
	void Lerp2CamY();
	void Lerp2CamZ();
	void Lerp2Perspective();
	void Lerp2Orthogonal();
	void CamMove(float spf);
private:

	UICanvas* canvas;
	Object* mainObj=nullptr;
	Object* grid3D = nullptr;
	DirectionalLight* dLight;
	Buffer* cbEye;

	//camera
	Camera* checkCam;
	const float camDist;
	float camViewRad;
	bool isLerping = false;
	const float lerpDuration=0.4f;
	float curLerpTime=0;
	float camAngleX = 0;
	float camAngleY = 0;
	float startAngleX;
	float startAngleY;
	float lerpAngleX;
	float lerpAngleY;

	//interface camera
	Camera* iCam;
};