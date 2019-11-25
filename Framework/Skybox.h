#pragma once
#include "Object.h"

class Buffer;

class Skybox : public Object
{
public:
	Skybox(ID3D11ShaderResourceView* srv);

	void Mapping();
	void Update() override;
	void Render(const XMMATRIX& parentWorld, const XMMATRIX& vp, UINT sceneDepth)const override;
	void RenderGeom() const override {};
	bool IsInsideFrustum(const Frustum& frustum) const override { return true; }

private:
	ComPtr<ID3D11SamplerState> samplerState;
};

