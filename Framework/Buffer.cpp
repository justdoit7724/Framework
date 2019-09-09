#include "Buffer.h"

Buffer::Buffer(D3D11_BUFFER_DESC* desc)
	:desc(*desc), isCB(false)
{
	r_assert(
		DX_Device->CreateBuffer(
			desc,
			nullptr,
			resource.GetAddressOf())
	);
}

Buffer::Buffer(D3D11_BUFFER_DESC* desc, void * initValue)
	:desc(*desc), isCB(false)
{
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = initValue;

	r_assert(
		DX_Device->CreateBuffer(
			desc,
			&data,
			resource.GetAddressOf())
	);
}

Buffer::Buffer(UINT byteSize)
	:isCB(true)
{
	UINT cbSize = 15 + byteSize - (byteSize - 1) % 16;

	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = cbSize;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.StructureByteStride = 0;
	desc.MiscFlags = 0;

	r_assert(
		DX_Device->CreateBuffer(
			&desc,
			nullptr,
			resource.GetAddressOf())
	);
}

void Buffer::SetSRV(D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc)
{
	assert(!isCB);

	r_assert(
		DX_Device->CreateShaderResourceView(
			resource.Get(),
			srvDesc,
			srv.GetAddressOf())
	);
}

void Buffer::SetUAV(D3D11_UNORDERED_ACCESS_VIEW_DESC * uavDesc)
{
	assert(!isCB);

	r_assert(
		DX_Device->CreateUnorderedAccessView(
			resource.Get(),
			uavDesc,
			uav.GetAddressOf())
	);
}

void Buffer::Write(void * data)
{
	assert(isCB);

	D3D11_MAPPED_SUBRESOURCE mappedData;

	r_assert(DX_DContext->Map(resource.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	CopyMemory(mappedData.pData, data, desc.ByteWidth);
	DX_DContext->Unmap(resource.Get(), 0);
}

