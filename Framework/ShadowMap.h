#pragma once
#include "DX_info.h"

class RasterizerState;
class DepthStencilState;
class BlendState;
class Object;
class VShader;
class Scene;
class DirectionalLight;
class Camera;
class Buffer;

class ShadowMap
{
public:
	ShadowMap(UINT resX, UINT resY, UINT width, UINT height);
	~ShadowMap();

	ID3D11ShaderResourceView* Depth();
	void Mapping(Scene* depthScene, const DirectionalLight* light);

private:
	ShadowMap(const ShadowMap& rhs);
	//ShadowMap& operator=(const ShadowMap& rhs);

	Camera* view;

	ComPtr<ID3D11ShaderResourceView> depthSRV;
	ComPtr<ID3D11DepthStencilView> depthDSV;
	ComPtr<ID3D11SamplerState> samp;
	RasterizerState* rsState;
	DepthStencilState* dsState;
	BlendState* blendState;
	VShader* mapVS;
	D3D11_VIEWPORT vp;
	Buffer* cbVPMat;
	
};

