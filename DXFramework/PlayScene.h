#pragma once

#include "Scene.h"

class DirectionalLight;
class Buffer;
class Camera;
class Object;

class PlayScene : public Scene
{
public:
	PlayScene();

	virtual void Update(float elapsed, float spf) override;

	void CreateModel(std::string filename);
	void CreateGrid();

private:

	
	void CamMove(float spf);

	Object* mainObj=nullptr;
	Object* grid3D = nullptr;
	Camera* checkCam;
	DirectionalLight* dLight;
	Buffer* cbEye;
	float camDist;
};