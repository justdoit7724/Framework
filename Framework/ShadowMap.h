#pragma once
#include "DX_info.h"

class RasterizerState;
class DepthStencilState;
class BlendState;
class Object;
class VShader;

class ShadowMap
{
public:
	ShadowMap(UINT width, UINT height);
	~ShadowMap();

	ID3D11ShaderResourceView* Depth();
	void Mapping(std::vector<Object*>& objs, const XMMATRIX& ptVPMat);

private:
	ShadowMap(const ShadowMap& rhs);
	//ShadowMap& operator=(const ShadowMap& rhs);


	UINT width;
	UINT height;

	ComPtr<ID3D11ShaderResourceView> depthSRV;
	ComPtr<ID3D11DepthStencilView> depthDSV;
	RasterizerState* rsState;
	DepthStencilState* dsState;
	BlendState* blendState;
	VShader* mapVS;
	D3D11_VIEWPORT vp;
};

