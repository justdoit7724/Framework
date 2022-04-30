
#include "pch.h"

#include "Mesh.h"
#include "ShaderFormat.h"
#include "Math.h"
#include "Vertex.h"

using namespace DX;

DX::Mesh::Mesh(ID3D11Device* device, Vertice vertice, const UINT* indice, UINT idxCount, D3D_PRIMITIVE_TOPOLOGY primitiveType)
{
	Init(device, vertice, indice, idxCount, primitiveType);
}

Mesh::~Mesh()
{
	m_vertexBuffer->Release();
	m_indexBuffer->Release();

	delete m_vertice;
}



void Mesh::CalculateTangent(XMFLOAT3 v0, XMFLOAT3 v1, XMFLOAT3 v2, XMFLOAT2 t0, XMFLOAT2 t1, XMFLOAT2 t2, OUT XMFLOAT3* tangent)
{
	XMFLOAT2 dt0 = t1 - t0;
	XMFLOAT2 dt1 = t2 - t0;
	XMFLOAT3 dv0 = v1 - v0;
	XMFLOAT3 dv1 = v2 - v0;
	float determinant = dt0.x * dt1.y - dt0.y * dt1.x;
	*tangent = dv0*(dt1.y / determinant) - dv1*(dt0.y / determinant);
}

void Mesh::GetLBound(OUT XMFLOAT3* minPt, OUT XMFLOAT3* maxPt)
{
	*minPt = m_lMinPt;
	*maxPt = m_lMaxPt;
}

void DX::Mesh::Init(ID3D11Device* device, Vertice vertice, const UINT* indice, UINT idxCount, D3D_PRIMITIVE_TOPOLOGY primitiveType)
{
	assert(m_vertexBuffer == nullptr);

	m_idxCount = idxCount;
	m_primitiveType = primitiveType;

	m_lMinPt = XMFLOAT3(FLT_MAX, FLT_MAX, FLT_MAX);
	m_lMaxPt = XMFLOAT3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	m_vertice = new Vertice(vertice.GetLayout());
	*m_vertice = vertice;
	
	for (int i = 0; i < vertice.Count(); ++i)
	{
		m_lMinPt.x = fminf(m_lMinPt.x, vertice[i].Attr< VertexLayout::ElementType::Position3D>().x);
		m_lMinPt.y = fminf(m_lMinPt.y, vertice[i].Attr< VertexLayout::ElementType::Position3D>().y);
		m_lMinPt.z = fminf(m_lMinPt.z, vertice[i].Attr< VertexLayout::ElementType::Position3D>().z);
		m_lMaxPt.x = fmaxf(m_lMaxPt.x, vertice[i].Attr< VertexLayout::ElementType::Position3D>().x);
		m_lMaxPt.y = fmaxf(m_lMaxPt.y, vertice[i].Attr< VertexLayout::ElementType::Position3D>().y);
		m_lMaxPt.z = fmaxf(m_lMaxPt.z, vertice[i].Attr< VertexLayout::ElementType::Position3D>().z);
	}

	// assimp calculates instead
	/*const int polyCount = idxCount / 3;
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
	}*/

	D3D11_BUFFER_DESC vb_desc;
	ZeroMemory(&vb_desc, sizeof(D3D11_BUFFER_DESC));
	vb_desc.Usage = D3D11_USAGE_IMMUTABLE;
	vb_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vb_desc.ByteWidth = vertice.SizeByte();
	vb_desc.CPUAccessFlags = 0;
	vb_desc.MiscFlags = 0;
	vb_desc.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vb_data;
	vb_data.pSysMem = vertice.GetData();
	HRESULT hr = device->CreateBuffer(
		&vb_desc,
		&vb_data,
		&m_vertexBuffer);
	r_assert(hr);

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * idxCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indice;
	hr = device->CreateBuffer(&ibd, &iinitData, &m_indexBuffer);
	r_assert(hr);
}

void Mesh::Apply(ID3D11DeviceContext* dContext)const
{
	dContext->IASetPrimitiveTopology(m_primitiveType);
	UINT offset = 0;
	UINT verticeSize = m_vertice->GetLayout().Size();
	dContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &verticeSize, &offset);
	dContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	dContext->DrawIndexed(m_idxCount, 0, 0);

}