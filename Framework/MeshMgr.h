#pragma once
#include "DX_info.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "Singleton.h"
#include <unordered_map>

class MeshMgr : public Singleton<MeshMgr>
{
public:
	void Add(std::string filename);
private:
	void ProcessNode(/*aiNode* node, const aiScene* scene*/);

	struct MeshInfo
	{
		ID3D11Buffer* vb;
		ID3D11Buffer* ib;
	};

	std::unordered_map<std::string, MeshInfo> meshes;
};

