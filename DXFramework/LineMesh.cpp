
#include "Mesh.h"
#include "ShaderFormat.h"
#include "LineMesh.h"

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

void LineMesh::Generate()
{
	isGenerated = true;

	std::vector<Vertex> vertice;
	std::vector<UINT> indice;

	for (int i = 0; i < lines.size(); ++i)
	{
		vertice.push_back(lines[i].a);
		vertice.push_back(lines[i].b);

		indice.push_back(i * 2);
		indice.push_back(i * 2+1);
	}

	Mesh::Init(vertice.data(), sizeof(Vertex), vertice.size(), indice.data(), indice.size(), D3D_PRIMITIVE_TOPOLOGY_LINELIST);
}

void LineMesh::Clear()
{
	lines.clear();
}

void LineMesh::Apply() const
{
	Mesh::Apply();
}
