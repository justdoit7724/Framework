
#include "pch.h"
#include "Buffer.h"

Buffer::Buffer(ID3D11Device* device, D3D11_BUFFER_DESC desc, const void * initValue)
	:desc(desc)
{
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = initValue;

	if (initValue == nullptr)
	{
		HRESULT hr = device->CreateBuffer(
				&desc,
				nullptr,
				&m_resource);
		if (FAILED(hr))
		{
			return;
		}
	}
	else
	{
		HRESULT hr = device->CreateBuffer(
				&desc,
				&data,
				&m_resource);
		if (FAILED(hr))
		{
			return;
		}
	}
}

UINT SizeCB(UINT byteSize)
{
	return (15 + byteSize - (byteSize - 1) % 16);
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
	if (FAILED(hr))
	{
		return;
	}
}
void Buffer::SetUAV(ID3D11Device* device, D3D11_UNORDERED_ACCESS_VIEW_DESC * uavDesc)
{
	if (m_uav)
		m_uav->Release();

	HRESULT hr = device->CreateUnorderedAccessView(
			m_resource,
			uavDesc,
			&m_uav);
	if (FAILED(hr))
	{
		return;
	}
}

void Buffer::Write(ID3D11DeviceContext* dContext, const void * data)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;

	dContext->Map(m_resource, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
	CopyMemory(mappedData.pData, data, desc.ByteWidth);
	dContext->Unmap(m_resource, 0);
}

void Buffer::GetSRV(ID3D11ShaderResourceView** srv)
{
	*srv = m_srv;
}

void Buffer::GetUAV(ID3D11UnorderedAccessView** uav)
{
	*uav = m_uav;
}

ConstantBuffer::ConstantBuffer(ID3D11Device* device, int byteSize)
	:Buffer(device, CD3D11_BUFFER_DESC(SizeCB(byteSize), D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE, NULL, NULL), nullptr)
{
}

VertexBuffer::VertexBuffer(ID3D11Device* device, const DX::Vertex* vertice, int verticeCount)
	: Buffer(device, CD3D11_BUFFER_DESC(verticeCount* sizeof(DX::Vertex), D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_IMMUTABLE, NULL, NULL, NULL), vertice),
	m_verticeCount(verticeCount)

{
}
BOOL VertexBuffer::Apply(ID3D11DeviceContext* dContext)
{
	UINT offset = 0;
	UINT stride = sizeof(DX::Vertex);
	dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	dContext->IASetVertexBuffers(0, 1, GetAddress(), &stride, &offset);
	return TRUE;
}
IndexBuffer::IndexBuffer(ID3D11Device* device, const UINT* indice, int indexCount)
	: Buffer(device, CD3D11_BUFFER_DESC(sizeof(UINT)* indexCount, D3D11_BIND_INDEX_BUFFER, D3D11_USAGE_IMMUTABLE, NULL, NULL, NULL), indice),
	m_indiceCount(indexCount)
{
}

BOOL IndexBuffer::Apply(ID3D11DeviceContext* dContext)
{
	dContext->IASetIndexBuffer(Get(), DXGI_FORMAT_R32_UINT, 0);
	dContext->DrawIndexed(m_indiceCount, 0, 0);

	return TRUE;
}
