#pragma once
#include "Resource.h"

class Texture2D : public Resource
{
public:
	Texture2D(D3D11_TEXTURE2D_DESC* desc);
	Texture2D(D3D11_TEXTURE2D_DESC* desc, void* data, UINT byteSize);

	void SetupSRV(D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc);
	void SetupUAV(D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc);
	void SetupRTV(D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc);
	ID3D11Texture2D * Get(){return tex.Get();}
	ID3D11ShaderResourceView*const* SRV();
	ID3D11UnorderedAccessView*const* UAV();
	ID3D11RenderTargetView*const* RTV();

	const D3D11_TEXTURE2D_DESC desc;
	
private:
	ComPtr<ID3D11Texture2D> tex=nullptr;
	ComPtr<ID3D11RenderTargetView> rtv=nullptr;
	ComPtr<ID3D11ShaderResourceView> srv=nullptr;
	ComPtr<ID3D11UnorderedAccessView> uav=nullptr;
};

