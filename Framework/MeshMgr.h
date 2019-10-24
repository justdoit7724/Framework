#pragma once
#include "DX_info.h"
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <unordered_map>
#include "Singleton.h"

struct Vertex;

struct MeshInfo
{
	Vertex* vertice;
	UINT* indice;
	UINT vCount;
	UINT iCount;
	aiString diffKey;
	aiString specKey;
	aiString normalKey;

	MeshInfo() {}
};

class MeshMgr : public Singleton<MeshMgr>
{
public:
	void Add(std::string key, std::string filepath);
	const std::vector<MeshInfo>& Get(std::string key);
	
private:
	std::unordered_map<std::string, std::vector<MeshInfo>> meshes;

	void ProcessNode(std::string key, aiNode* node, const aiScene* scene);
	MeshInfo ProcessMesh(aiMesh* mesh, const aiMaterial* const* materials);

};

