#pragma once
#include "DX_info.h"

class Texture2D
{
public:
	Texture2D(ID3D11Device* device, D3D11_TEXTURE2D_DESC* desc);
	Texture2D(ID3D11Device* device, ID3D11DeviceContext* dContext, D3D11_TEXTURE2D_DESC* desc, void* data, UINT byteSize);

	void SetupSRV(ID3D11Device* device, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc);
	void SetupUAV(ID3D11Device* device, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc);
	void SetupRTV(ID3D11Device* device, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc);
	ID3D11Texture2D * Get(){return tex.Get();}
	ID3D11ShaderResourceView*const* SRV(ID3D11Device* device);
	ID3D11UnorderedAccessView*const* UAV(ID3D11Device* device);
	ID3D11RenderTargetView*const* RTV(ID3D11Device* device);

	const D3D11_TEXTURE2D_DESC desc;
	static ID3D11ShaderResourceView*const nullSrv;
	static ID3D11UnorderedAccessView*const nullUav;
	static ID3D11RenderTargetView*const nullRtv;
	
private:
	ComPtr<ID3D11Texture2D> tex=nullptr;
	ComPtr<ID3D11RenderTargetView> rtv=nullptr;
	ComPtr<ID3D11ShaderResourceView> srv=nullptr;
	ComPtr<ID3D11UnorderedAccessView> uav=nullptr;
};

