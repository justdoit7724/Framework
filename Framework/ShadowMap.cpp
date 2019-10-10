#include "ShadowMap.h"

ShadowMap::ShadowMap(UINT width, UINT height)
	:width(width), height(height), depthSRV(nullptr), depthDSV(nullptr)
{
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	D3D11_TEXTURE2D_DESC tex_desc;
	tex_desc.Width = width;
	tex_desc.Height = height;
	tex_desc.MipLevels = 1;
	tex_desc.ArraySize = 1;
	tex_desc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	tex_desc.SampleDesc = { 1,0 };
	tex_desc.Usage = D3D11_USAGE_DEFAULT;
	tex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	tex_desc.CPUAccessFlags = 0;
	tex_desc.MiscFlags = 0;
	ComPtr<ID3D11Texture2D> tex;
	r_assert(
		DX_Device->CreateTexture2D(&tex_desc, nullptr, tex.GetAddressOf())
	);

	D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc;
	dsv_desc.Flags = 0;
	dsv_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsv_desc.Texture2D.MipSlice = 0;
	r_assert(
		DX_Device->CreateDepthStencilView(tex.Get(), &dsv_desc, depthDSV.GetAddressOf())
	);

	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
	srv_desc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv_desc.Texture2D.MipLevels = 1;
	srv_desc.Texture2D.MostDetailedMip = 0;
	r_assert(
		DX_Device->CreateShaderResourceView(tex.Get(), &srv_desc, depthSRV.GetAddressOf())
	);
}

ShadowMap::~ShadowMap()
{
}

ID3D11ShaderResourceView* ShadowMap::Depth()
{
	return depthSRV.Get();
}

void ShadowMap::BindDSVAndSetNullRT()
{
	DX_DContext->RSSetViewports(1, &vp);

	ID3D11RenderTargetView* nullRTV = nullptr;
	DX_DContext->OMSetRenderTargets(1, &nullRTV, depthDSV.Get());
}

//ShadowMap& ShadowMap::operator=(const ShadowMap& rhs)
//{
//	// TODO: insert return statement here
//}
