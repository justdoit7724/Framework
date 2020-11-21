
#include "pch.h"
#include "Buffer.h"

using namespace DX;
Buffer::Buffer(ID3D11Device* device, D3D11_BUFFER_DESC* desc, void * initValue)
	:desc(*desc)
{
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = initValue;

	if (initValue == nullptr)
	{
		HRESULT hr = device->CreateBuffer(
				desc,
				nullptr,
				&resource);
		r_assert(hr);
	}
	else
	{
		HRESULT hr = device->CreateBuffer(
				desc,
				&data,
				&resource);
		r_assert(hr);
	}
}

UINT SizeCB(UINT byteSize)
{
	return (15 + byteSize - (byteSize - 1) % 16);
}
Buffer::Buffer(ID3D11Device* device, UINT byteSize)
	:desc(CD3D11_BUFFER_DESC(SizeCB(byteSize), D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE, 0, 0))
{
	HRESULT hr = device->CreateBuffer(&desc, nullptr, &resource);
	r_assert(hr);
}

Buffer::~Buffer()
{
	resource->Release();
	if(srv)
		srv->Release();
	if(uav)
		uav->Release();
}


void Buffer::SetSRV(ID3D11Device* device, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc)
{
	HRESULT hr = device->CreateShaderResourceView(
			resource,
			srvDesc,
			&srv);
	r_assert(hr);
}
void Buffer::SetUAV(ID3D11Device* device, D3D11_UNORDERED_ACCESS_VIEW_DESC * uavDesc)
{
	HRESULT hr = device->CreateUnorderedAccessView(
			resource,
			uavDesc,
			&uav);
	r_assert(hr);
}

void Buffer::Write(ID3D11DeviceContext* dContext, const void * data)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;

	dContext->Map(resource, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
	CopyMemory(mappedData.pData, data, desc.ByteWidth);
	dContext->Unmap(resource, 0);
}
