#pragma once
#include "DX_info.h"

class Camera;

class Scene
{
public:
	Scene(std::string key);
	~Scene();

	virtual void Update(Camera* camera)=0;
	virtual void Render()const=0;

	const std::string key;
};

