
#include "pch.h"

#include "Mesh.h"
#include "ShaderFormat.h"
#include "LineMesh.h"

using namespace DX;

LineMesh::LineMesh()
	:Mesh(1)
{
}

void LineMesh::Add(XMFLOAT3 a, XMFLOAT3 b)
{
	LineInfo newLine;
	newLine.a = a;
	newLine.b = b;

	lines.push_back(newLine);
}

<<<<<<< HEAD
void LineMesh::Generate(ID3D11Device* device, UINT id)
{
	isGenerated = true;

	std::vector<Vertex> vertice;
	std::vector<UINT> indice;*///8
=======
void LineMesh::Generate(ID3D11Device* device, const VertexLayout* layout)
{
	isGenerated = true;

	Vertice vertice(*layout);
	std::vector<UINT> indice;
>>>>>>> 03_DepthPeeling

	for (int i = 0; i < lines.size(); ++i)
	{
		vertice.EmplaceBack();
		vertice.EmplaceBack();

		vertice[i * 2].Attr<VertexLayout::ElementType::Position3D>() = lines[i].a;
		vertice[i * 2+1].Attr<VertexLayout::ElementType::Position3D>() = lines[i].b;

		indice.push_back(i * 2);
		indice.push_back(i * 2+1);
	}

<<<<<<< HEAD
	Mesh::SetSubMesh(device, 0, vertice.data(), sizeof(Vertex), vertice.size(), indice.data(), indice.size(), D3D_PRIMITIVE_TOPOLOGY_LINELIST);
=======
	Mesh::Init(device, vertice, indice.data(), indice.size(), D3D_PRIMITIVE_TOPOLOGY_LINELIST);
>>>>>>> 03_DepthPeeling
}

void LineMesh::Clear()
{
	lines.clear();
}

void LineMesh::Apply(ID3D11DeviceContext* dContext) const
{
	Mesh::Apply(dContext);
}
