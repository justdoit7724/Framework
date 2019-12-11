#pragma once

#include "DX_info.h"

class RasterizerState;
class DepthStencilState;
class BlendState;
class Object;
class VShader;
class PShader;
class DirectionalLight;
class Camera;
class Buffer;

class OpaqueShadowMap
{
public:
	OpaqueShadowMap(UINT resX, UINT resY, UINT width, UINT height, const std::vector<Object const*>& objs);
	~OpaqueShadowMap();

	ID3D11ShaderResourceView* Depth();
	void Mapping(const DirectionalLight* light);

private:
	OpaqueShadowMap(const OpaqueShadowMap& rhs);
	//ShadowMap& operator=(const ShadowMap& rhs);

	Camera* view;

	ComPtr<ID3D11ShaderResourceView> depthSRV;
	ComPtr<ID3D11DepthStencilView> depthDSV;
	RasterizerState* rsState;
	DepthStencilState* dsState;
	BlendState* blendState;
	VShader* mapVS;
	D3D11_VIEWPORT vp;
	Buffer* cbVPMat;

	Object const* drawObjs[3];
};

class TranspShadowMap
{
public:
	TranspShadowMap(XMUINT2 res, XMUINT2 volume, const std::vector<Object const*>& objs);
	~TranspShadowMap();

	void Mapping(const DirectionalLight* dLight);
	ID3D11ShaderResourceView* SRV() { return srv.Get(); }
private:
	ComPtr<ID3D11RenderTargetView> rtv;
	ComPtr<ID3D11ShaderResourceView> srv;
	ComPtr<ID3D11DepthStencilView> dsv;
	DepthStencilState* dsState;
	RasterizerState* rsState;
	BlendState* blendState;

	Camera* view;

	D3D11_VIEWPORT vp;
	VShader* mapVS;
	PShader* mapPS;
	Buffer* cbVPTMat;

	Object const* drawObjs[3];
};

