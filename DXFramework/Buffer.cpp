
#include "Buffer.h"

Buffer::Buffer(D3D11_BUFFER_DESC* desc, void * initValue)
	:desc(*desc)
{
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = initValue;

	if (initValue == nullptr)
	{
		HRESULT hr = DX_Device->CreateBuffer(
				desc,
				nullptr,
				resource.GetAddressOf());
		r_assert(hr);
	}
	else
	{
		HRESULT hr = DX_Device->CreateBuffer(
				desc,
				&data,
				resource.GetAddressOf());
		r_assert(hr);
	}
}

UINT SizeCB(UINT byteSize)
{
	return (15 + byteSize - (byteSize - 1) % 16);
}
Buffer::Buffer(UINT byteSize)
	:desc(CD3D11_BUFFER_DESC(SizeCB(byteSize), D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE, 0, 0))
{
	HRESULT hr = DX_Device->CreateBuffer(&desc, nullptr, resource.GetAddressOf());
	r_assert(hr);
}


void Buffer::SetSRV(D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc)
{
	HRESULT hr = DX_Device->CreateShaderResourceView(
			resource.Get(),
			srvDesc,
			srv.GetAddressOf());
	r_assert(hr);
}
void Buffer::SetUAV(D3D11_UNORDERED_ACCESS_VIEW_DESC * uavDesc)
{
	HRESULT hr = DX_Device->CreateUnorderedAccessView(
			resource.Get(),
			uavDesc,
			uav.GetAddressOf());
	r_assert(hr);
}

void Buffer::Write(const void * data)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;

	DX_DContext->Map(resource.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
	CopyMemory(mappedData.pData, data, desc.ByteWidth);
	DX_DContext->Unmap(resource.Get(), 0);
}
