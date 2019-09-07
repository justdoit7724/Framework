#include "Buffer.h"

Buffer::Buffer(ID3D11Device * device, const CD3D11_BUFFER_DESC* desc)
	:desc(*desc)
{
	r_assert(
		device->CreateBuffer(
			desc,
			nullptr,
			resource.GetAddressOf())
	);
}

Buffer::Buffer(ID3D11Device * device, const CD3D11_BUFFER_DESC* desc, void * initValue)
	:desc(*desc)
{
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = initValue;

	r_assert(
		device->CreateBuffer(
			desc,
			&data,
			resource.GetAddressOf())
	);
}

void Buffer::SetSRV(ID3D11Device * device, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc)
{
	r_assert(
		device->CreateShaderResourceView(
			resource.Get(),
			srvDesc,
			srv.GetAddressOf())
	);
}

void Buffer::SetUAV(ID3D11Device * device, D3D11_UNORDERED_ACCESS_VIEW_DESC * uavDesc)
{
	r_assert(
		device->CreateUnorderedAccessView(
			resource.Get(),
			uavDesc,
			uav.GetAddressOf())
	);
}

void Buffer::Update(ID3D11DeviceContext * dContext, void * data, UINT byteSize)
{
	MB("Check if works first !!");

	D3D11_MAPPED_SUBRESOURCE mappedData;

	dContext->Map(resource.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
	CopyMemory(mappedData.pData, data, byteSize);
	dContext->Unmap(resource.Get(), 0);
}
