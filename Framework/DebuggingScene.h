#pragma once
#include "Scene.h"
class DebuggingScene :
	public Scene
{
public:
	DebuggingScene();
	~DebuggingScene();

	void Update(float elapsed, float spf)override;
	void Render(const XMMATRIX& vp, const Frustum& frustum, UINT sceneDepth, void const* subject)const override;
};