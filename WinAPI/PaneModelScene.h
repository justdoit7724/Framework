#pragma once
#include "Scene.h"
#include "Math.h"

class Object;

class PaneModelScene :
	public Scene
{
public:
	PaneModelScene(DX::Graphic* graphic);
	~PaneModelScene()override;

	void Update() override;

	void Open(std::string path);
	void CreateGrid(int nX, int nY, XMFLOAT3 size);

private:
	Object* m_light;
};

