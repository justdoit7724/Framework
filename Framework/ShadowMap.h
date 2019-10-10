#pragma once
#include "DX_info.h"

class ShadowMap
{
public:
	ShadowMap(UINT width, UINT height);
	~ShadowMap();

	ID3D11ShaderResourceView* Depth();
	void BindDSVAndSetNullRT();

private:
	ShadowMap(const ShadowMap& rhs);
	//ShadowMap& operator=(const ShadowMap& rhs);

	UINT width;
	UINT height;

	ComPtr<ID3D11ShaderResourceView> depthSRV;
	ComPtr<ID3D11DepthStencilView> depthDSV;
	D3D11_VIEWPORT vp;
};

