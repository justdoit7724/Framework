#include "Shape.h"
#include "CustomFormat.h"

Shape::Shape()
{
}

void Shape::Init(const Vertex* vertice, const int vertexCount, const UINT* indice, const int idxCount)
{
	indexCount = idxCount;

	D3D11_BUFFER_DESC vb_desc;
	ZeroMemory(&vb_desc, sizeof(D3D11_BUFFER_DESC));
	vb_desc.Usage = D3D11_USAGE_IMMUTABLE;
	vb_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vb_desc.ByteWidth = sizeof(Vertex) * vertexCount;
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

void Shape::Apply()
{
	DX_DContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	DX_DContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	DX_DContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	DX_DContext->DrawIndexed(indexCount, 0, 0);
}