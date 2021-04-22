#pragma once

#include "Object.h"

class Skybox : public Object
{
	/*
public:
	Skybox(ID3D11Device* device, ID3D11DeviceContext* dContext);
	~Skybox();

	void Mapping(ID3D11DeviceContext* dContext, ID3D11ShaderResourceView* cmSRV)const;
	void Update() override {};
	void Render(ID3D11DeviceContext* dContext, const XMMATRIX& v, const XMMATRIX& p, const Frustum* frustum, UINT sceneDepth = 0)const override;
	void RenderGeom(ID3D11DeviceContext* dContext) const override {};

private:
	void Blur()const;
	bool IsInsideFrustum(const Frustum* frustum) const override { return true; }
	ID3D11SamplerState* samplerState;

	ID3D11ShaderResourceView* blur1SRV, * blur2SRV;
	ID3D11RenderTargetView* blur1RTV, * blur2RTV;
	//ID3D11DepthStencilView> 
	*/
};
