#pragma once
#include "Scene.h"

#define N 6

class Buffer;
class Object;
class DirectionalLight;

class TestScene : public Scene
{
public:
	TestScene();
	~TestScene();

	void Update(float elapsed, float spf)override;
private:
	Object* objs[N][N][N];
	Buffer* cbEye;
	DirectionalLight* dLight;
};