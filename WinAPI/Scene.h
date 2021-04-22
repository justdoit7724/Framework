#pragma once
#include "WndDisplay.h"


class Object;

class Scene
{
public:
	virtual ~Scene();

protected:
	Scene() = delete;
	Scene(DX::Graphic* graphic, std::string key);

public:
	virtual void Update();
	virtual void Render();

	const std::string m_strKey;
protected:
	DX::Graphic* m_dxGraphic;

	std::vector<Object*> m_vRootObj;


};


