
#include "pch.h"

#include "Mesh.h"
#include "ShaderFormat.h"
#include "LineMesh.h"

using namespace DX;

LineMesh::LineMesh()
{
}

void LineMesh::Add(XMFLOAT3 a, XMFLOAT3 b)
{
	LineInfo newLine;
	newLine.a = a;
	newLine.b = b;

	lines.push_back(newLine);
}

void LineMesh::Generate(ID3D11Device* device, const VertexLayout* layout)
{
	isGenerated = true;

	Vertice vertice(*layout);
	std::vector<UINT> indice;

	for (int i = 0; i < lines.size(); ++i)
	{
		vertice.EmplaceBack();
		vertice.EmplaceBack();

		vertice[i * 2].Attr<VertexLayout::ElementType::Position3D>() = lines[i].a;
		vertice[i * 2+1].Attr<VertexLayout::ElementType::Position3D>() = lines[i].b;

		indice.push_back(i * 2);
		indice.push_back(i * 2+1);
	}

	Mesh::Init(device, vertice, indice.data(), indice.size(), D3D_PRIMITIVE_TOPOLOGY_LINELIST);
}

void LineMesh::Clear()
{
	lines.clear();
}

void LineMesh::Apply(ID3D11DeviceContext* dContext) const
{
	Mesh::Apply(dContext);
}
