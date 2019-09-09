#include "Buffer.h"

Buffer::Buffer(D3D11_BUFFER_DESC* desc, void * initValue)
	:desc(*desc)
{
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = initValue;

	if (initValue == nullptr)
	{
		r_assert(
			device->CreateBuffer(
				desc,
				nullptr,
				resource.GetAddressOf())
		);
	}
	else
	{
		r_assert(
			device->CreateBuffer(
				desc,
				&data,
				resource.GetAddressOf())
		);
	}
}

UINT SizeCB(UINT byteSize)
{
	return (15 + byteSize - (byteSize - 1) % 16);
}
Buffer::Buffer(UINT byteSize)
	:desc(CD3D11_BUFFER_DESC(SizeCB(byteSize), D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE, 0, 0))
{
	r_assert(
		device->CreateBuffer(&desc, nullptr, resource.GetAddressOf())
	);
}


void Buffer::SetSRV(D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc)
{
	r_assert(
		device->CreateShaderResourceView(
			resource.Get(),
			srvDesc,
			srv.GetAddressOf())
	);
}
void Buffer::SetUAV(D3D11_UNORDERED_ACCESS_VIEW_DESC * uavDesc)
{
	r_assert(
		device->CreateUnorderedAccessView(
			resource.Get(),
			uavDesc,
			uav.GetAddressOf())
	);
}

void Buffer::Write(void * data)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;

	dContext->Map(resource.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
	CopyMemory(mappedData.pData, data, desc.ByteWidth);
	dContext->Unmap(resource.Get(), 0);
}
