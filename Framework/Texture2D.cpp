#include "Texture2D.h"

//delete
#include <vector>

Texture2D::Texture2D(D3D11_TEXTURE2D_DESC * desc)
	:desc(*desc)
{
	r_assert(
		device->CreateTexture2D(desc, nullptr, tex.GetAddressOf())
	);
}

Texture2D::Texture2D(D3D11_TEXTURE2D_DESC * desc, void * data, UINT typeByteSize)
	:desc(*desc)
{
	D3D11_SUBRESOURCE_DATA subData;
	subData.pSysMem = data;
	subData.SysMemPitch = typeByteSize * desc->Width;

	r_assert(
		device->CreateTexture2D(desc, &subData, tex.GetAddressOf())
	);
}

void Texture2D::SetupSRV(D3D11_SHADER_RESOURCE_VIEW_DESC * srvDesc)
{
	if (srv)
	{
		srv->Release();
	}

	r_assert(
		device->CreateShaderResourceView(tex.Get(), srvDesc, srv.GetAddressOf())
	);
}

void Texture2D::SetupUAV(D3D11_UNORDERED_ACCESS_VIEW_DESC * uavDesc)
{
	if (uav)
	{
		uav->Release();
	}

	r_assert(
		device->CreateUnorderedAccessView(tex.Get(), uavDesc, uav.GetAddressOf())
	);
}

void Texture2D::SetupRTV(D3D11_RENDER_TARGET_VIEW_DESC * rtvDesc)
{
	if (rtv)
	{
		rtv->Release();
	}

	r_assert(
		device->CreateRenderTargetView(tex.Get(), rtvDesc, rtv.GetAddressOf())
	);
}

ID3D11ShaderResourceView*const* Texture2D::SRV()
{
	assert(srv != nullptr);

	return srv.GetAddressOf();
}

ID3D11UnorderedAccessView *const* Texture2D::UAV()
{
	assert(uav != nullptr);

	return uav.GetAddressOf();
}

ID3D11RenderTargetView*const* Texture2D::RTV()
{
	assert(rtv != nullptr);

	return rtv.GetAddressOf();
}
