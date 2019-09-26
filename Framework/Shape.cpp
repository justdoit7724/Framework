#include "Shape.h"
#include "CustomFormat.h"

Shape::Shape()
{
}

void Shape::Init(void* vertice, UINT _vertexByteSize, UINT vertexCount, void* indice, UINT _idxCount, D3D_PRIMITIVE_TOPOLOGY _primitiveType)
{
	indexCount = _idxCount;
	vertByteSize = _vertexByteSize;
	primitiveType = _primitiveType;

	if (vertexBuffer)
	{
		vertexBuffer->Release();
	}
	if (indexBuffer)
	{
		indexBuffer->Release();
	}

	D3D11_BUFFER_DESC vb_desc;
	ZeroMemory(&vb_desc, sizeof(D3D11_BUFFER_DESC));
	vb_desc.Usage = D3D11_USAGE_IMMUTABLE;
	vb_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vb_desc.ByteWidth = vertByteSize * vertexCount;
	vb_desc.CPUAccessFlags = 0;
	vb_desc.MiscFlags = 0;
	vb_desc.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vb_data;
	vb_data.pSysMem = vertice;
	r_assert(
		DX_Device->CreateBuffer(
			&vb_desc,
			&vb_data,
			vertexBuffer.GetAddressOf())
	);

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * indexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indice;
	r_assert(
		DX_Device->CreateBuffer(&ibd, &iinitData, indexBuffer.GetAddressOf())
	);
}

void Shape::Apply()const
{
	DX_DContext->IASetPrimitiveTopology(primitiveType);
	UINT offset = 0;
	DX_DContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &vertByteSize, &offset);
	DX_DContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	DX_DContext->DrawIndexed(indexCount, 0, 0);
}