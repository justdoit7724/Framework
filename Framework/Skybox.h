#pragma once

#include "Object.h"

class Buffer;

class Skybox : public Object
{
public:
	Skybox();

	void Mapping()const;
	void Update() override {};
	void Render(const XMMATRIX& vp, const Frustum& frustum, UINT sceneDepth)const override;
	void RenderGeom() const override {};

private:
	void Blur()const;
	bool IsInsideFrustum(const Frustum& frustum) const override { return true; }
	ComPtr<ID3D11SamplerState> samplerState;

	ComPtr<ID3D11ShaderResourceView> blur1SRV, blur2SRV;
	ComPtr<ID3D11RenderTargetView> blur1RTV, blur2RTV;
	//ComPtr<ID3D11DepthStencilView> 
};

