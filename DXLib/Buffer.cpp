
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
				&m_resource);
		r_assert(hr);
	}
	else
	{
		HRESULT hr = device->CreateBuffer(
				desc,
				&data,
				&m_resource);
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
	HRESULT hr = device->CreateBuffer(&desc, nullptr, &m_resource);
	r_assert(hr);
}

Buffer::~Buffer()
{
	m_resource->Release();
	if(m_srv)
		m_srv->Release();
	if(m_uav)
		m_uav->Release();
}


void Buffer::SetSRV(ID3D11Device* device, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc)
{
	if (m_srv)
		m_srv->Release();

	HRESULT hr = device->CreateShaderResourceView(
			m_resource,
			srvDesc,
			&m_srv);
	r_assert(hr);
}
void Buffer::SetUAV(ID3D11Device* device, D3D11_UNORDERED_ACCESS_VIEW_DESC * uavDesc)
{
	if (m_uav)
		m_uav->Release();

	HRESULT hr = device->CreateUnorderedAccessView(
			m_resource,
			uavDesc,
			&m_uav);
	r_assert(hr);
}

void Buffer::Write(ID3D11DeviceContext* dContext, const void * data)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;

	dContext->Map(m_resource, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
	CopyMemory(mappedData.pData, data, desc.ByteWidth);
	dContext->Unmap(m_resource, 0);
}

void DX::Buffer::GetSRV(ID3D11ShaderResourceView** srv)
{
	*srv = m_srv;
}

void DX::Buffer::GetUAV(ID3D11UnorderedAccessView** uav)
{
	*uav = m_uav;
}
