#include "stdafx.h"
#include "Mesh.h"

SubMesh::SubMesh(const std::string key, const std::vector<DX::Vertex>& vertice, const std::vector<UINT>& indice)
	:
	m_key(key),
	m_vertice(vertice),
	m_indice(indice)
{
}



void Mesh::SetSubMesh(int id, SubMesh* subMesh)
{
	m_subMeshes.reserve(id + 1);

	m_subMeshes[id] = subMesh;
}

const std::vector<SubMesh*>& Mesh::GetSubMesh()
{
	return m_subMeshes;
}
