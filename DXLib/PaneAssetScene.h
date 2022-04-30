#pragma once
#include "Scene.h"
#include "Math.h"

class Object;

class PaneAssetScene :
	public Scene
{
public:
	PaneAssetScene(DX::Graphic* graphic);
	~PaneAssetScene()override;

	void Update() override;

	void Open(std::string path);
	void CreateGrid(int nX, int nY, XMFLOAT3 size);

private:
	Object* m_light;
};

