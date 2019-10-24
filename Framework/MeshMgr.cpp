#include "MeshMgr.h"
#include "ShaderFormat.h"
#include <mutex>

void MeshMgr::Add(std::string key, std::string filename)
{
	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile("Data\\Model\\"+ filename,
		aiProcess_ConvertToLeftHanded);

	assert(
		pScene != nullptr &&
		pScene->HasMaterials() &&
		pScene->HasMeshes());

	meshes.insert(std::pair < std::string, std::vector<MeshInfo>>(key, std::vector<MeshInfo>()));
	ProcessNode(key, pScene->mRootNode, pScene);
}

const std::vector<MeshInfo>& MeshMgr::Get(std::string key)
{
	assert(meshes.find(key) != meshes.end());

	return meshes[key];
}

void MeshMgr::ProcessNode(std::string key, aiNode* node, const aiScene* scene)
{
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		const aiMaterial* const material = scene->mMaterials[mesh->mMaterialIndex];

		assert(mesh->HasNormals());
		assert(mesh->mMaterialIndex >= 0);
		
		MeshInfo meshInfo;

		aiString textureName;
		assert(material->GetTexture(aiTextureType_DIFFUSE, 0, &textureName) == aiReturn_SUCCESS);
		meshInfo.diffMtl = new std::string();
		*meshInfo.diffMtl = textureName.C_Str();
		assert(material->GetTexture(aiTextureType_HEIGHT, 0, &textureName) == aiReturn_SUCCESS);
		meshInfo.normalMtl = new std::string();
		*meshInfo.normalMtl = textureName.C_Str();
		if (material->GetTexture(aiTextureType_SPECULAR, 0, &textureName) == aiReturn_SUCCESS)
		{
			meshInfo.specMtl = new std::string();
			*meshInfo.specMtl = textureName.C_Str();
		}

		meshInfo.vCount = mesh->mNumVertices;
		meshInfo.iCount = mesh->mNumFaces * 3;
		meshInfo.vertice = new Vertex[meshInfo.vCount];
		meshInfo.indice = new UINT[meshInfo.iCount];

		for (int i = 0; i < mesh->mNumVertices; ++i)
		{
			Vertex curVertex;
			curVertex.pos = XMFLOAT3(
				mesh->mVertices[i].x,
				mesh->mVertices[i].y,
				mesh->mVertices[i].z);
			curVertex.n = XMFLOAT3(
				mesh->mNormals[i].x,
				mesh->mNormals[i].y,
				mesh->mNormals[i].z);
			curVertex.tex = XMFLOAT2(
				mesh->mTextureCoords[0][i].x,
				mesh->mTextureCoords[0][i].y);
			if (curVertex.tex.x < 0 || curVertex.tex.x > 1 || curVertex.tex.y < 0 || curVertex.tex.y > 1)
			{
				int a = 0;
				int b = a;
			}

			meshInfo.vertice[i] = curVertex;
		}
		for (int i = 0; i < mesh->mNumFaces; ++i)
		{
			aiFace face = mesh->mFaces[i];

			for (int j = 0; j < face.mNumIndices; ++j)
			{
				assert(face.mNumIndices == 3);
				meshInfo.indice[i * 3 + j] = face.mIndices[j];
			}
		}

		meshes[key].push_back(meshInfo);
	}

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		this->ProcessNode(key, node->mChildren[i], scene);
	}
}
