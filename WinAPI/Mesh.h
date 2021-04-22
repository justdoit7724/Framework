#pragma once
#include "Asset.h"


class SubMesh
{
public:
	SubMesh(const std::string key, const std::vector<DX::Vertex>& vertice, const std::vector<UINT>& indice);

	const std::string m_key;
	const std::vector<DX::Vertex> m_vertice;
	const std::vector<UINT> m_indice;
};


class Mesh :
    public Asset
{
public:

	void SetSubMesh(int id, SubMesh* subMesh);
	const std::vector<SubMesh*>& GetSubMesh();

private:
	std::vector<SubMesh*> m_subMeshes;
    
};

