#include "Shape.h"
#include "ShaderFormat.h"

Shape::Shape(const Vertex* vertice, UINT vertByteSize, UINT vertCount, const UINT* indice, UINT idxCount, D3D_PRIMITIVE_TOPOLOGY primitiveType)
{
	Init(vertice, vertByteSize, vertCount, indice, idxCount, primitiveType);
}

void Shape::Init(const Vertex* vertice, UINT vertByteSize, UINT vertCount, const UINT* indice, UINT idxCount, D3D_PRIMITIVE_TOPOLOGY primitiveType)
{
	assert(vertexBuffer == nullptr);

	this->idxCount = idxCount;
	this->vertByteSize = vertByteSize;
	this->primitiveType = primitiveType;

	lMinPt = XMFLOAT3(FLT_MAX, FLT_MAX, FLT_MAX);
	lMaxPt = XMFLOAT3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	for (int i = 0; i < vertCount; ++i)
	{
		lMinPt.x = fminf(lMinPt.x, vertice[i].pos.x);
		lMinPt.y = fminf(lMinPt.y, vertice[i].pos.y);
		lMinPt.z = fminf(lMinPt.z, vertice[i].pos.z);
		lMaxPt.x = fmaxf(lMaxPt.x, vertice[i].pos.x);
		lMaxPt.y = fmaxf(lMaxPt.y, vertice[i].pos.y);
		lMaxPt.z = fmaxf(lMaxPt.z, vertice[i].pos.z);
	}

	D3D11_BUFFER_DESC vb_desc;
	ZeroMemory(&vb_desc, sizeof(D3D11_BUFFER_DESC));
	vb_desc.Usage = D3D11_USAGE_IMMUTABLE;
	vb_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vb_desc.ByteWidth = vertByteSize * vertCount;
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
	ibd.ByteWidth = sizeof(UINT) * idxCount;
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

void Shape::CalculateTangent(XMFLOAT3 v0, XMFLOAT3 v1, XMFLOAT3 v2, XMFLOAT2 t0, XMFLOAT2 t1, XMFLOAT2 t2, OUT XMFLOAT3* tangent)
{
	XMFLOAT2 dt0 = t1 - t0;
	XMFLOAT2 dt1 = t2 - t0;
	XMFLOAT3 dv0 = v1 - v0;
	XMFLOAT3 dv1 = v2 - v0;
	float determinant = dt0.x * dt1.y - dt0.y * dt1.x;
	*tangent = dv0*(dt1.y / determinant) - dv1*(dt0.y / determinant);
}

void Shape::GetLBound(OUT XMFLOAT3* minPt, OUT XMFLOAT3* maxPt)
{
	*minPt = lMinPt;
	*maxPt = lMaxPt;
}

void Shape::CalculateTangents(Vertex* vertice, const UINT* indice, UINT polyCount)
{
	for (int i = 0; i < polyCount; ++i)
	{
		XMFLOAT3 v0 = vertice[indice[i * 3]].pos;
		XMFLOAT3 v1 = vertice[indice[i * 3 + 1]].pos;
		XMFLOAT3 v2 = vertice[indice[i * 3 + 2]].pos;
		XMFLOAT2 t0 = vertice[indice[i * 3]].tex;
		XMFLOAT2 t1 = vertice[indice[i * 3 + 1]].tex;
		XMFLOAT2 t2 = vertice[indice[i * 3 + 2]].tex;
		XMFLOAT3 tangent = XMFLOAT3(0, 0, 0);
		CalculateTangent(v0, v1, v2, t0, t1, t2, &tangent);
		vertice[indice[i * 3]].tangent = tangent;
		vertice[indice[i * 3 + 1]].tangent = tangent;
		vertice[indice[i * 3 + 2]].tangent = tangent;
	}
}

void Shape::Apply()const
{
	DX_DContext->IASetPrimitiveTopology(primitiveType);
	UINT offset = 0;
	DX_DContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &vertByteSize, &offset);
	DX_DContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	DX_DContext->DrawIndexed(idxCount, 0, 0);

}