
#include "pch.h"

#include "MeshLoader.h"
#include "ShaderFormat.h"
#include "Object.h"
#include "TextureMgr.h"
#include "Shader.h"
#include "Mesh.h"
#include <mutex>

#include "_Packages\Assimp.3.0.0\include\postprocess.h"
#include "_Packages\Assimp.3.0.0\include\Importer.hpp"
#include "_Packages\Assimp.3.0.0\include\scene.h"

using namespace DX;


void ProcessNode(std::vector<Mesh*>* storage, std::string filepath, aiNode* node, const aiScene* scene)
{
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		const aiMaterial* const material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<Vertex> vertice(mesh->mNumVertices);
		std::vector<UINT> indice(mesh->mNumFaces*3);
		
		for (int j = 0; j < mesh->mNumVertices; ++j)
		{
			vertice[j].pos = XMFLOAT3(
				mesh->mVertices[j].x,
				mesh->mVertices[j].y,
				mesh->mVertices[j].z);
			vertice[j].n = XMFLOAT3(
				mesh->mNormals[j].x,
				mesh->mNormals[j].y,
				mesh->mNormals[j].z);
		}
		for (int i = 0; i < mesh->mNumFaces; ++i)
		{
			aiFace face = mesh->mFaces[i];

			for (int j = 0; j < face.mNumIndices; ++j)
			{
				assert(face.mNumIndices == 3 && "should be composed of only POLYGON");
				indice[(i * 3) + j] = face.mIndices[j];
			}
		}

		storage->push_back(new Mesh(vertice.data(), sizeof(Vertex), vertice.size(), indice.data(), indice.size(), D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(storage,filepath, node->mChildren[i], scene);
	}
}

void MeshLoader::Load(std::vector<Mesh*>* storage, std::string filepath, std::string filename)
{
	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(filepath+filename,
		aiProcess_MakeLeftHanded|
		aiProcess_FlipUVs|
		aiProcess_FlipWindingOrder|
		aiProcess_GenNormals);

	assert(
		pScene != nullptr &&
		pScene->HasMeshes());

	ProcessNode(storage, filepath, pScene->mRootNode, pScene);
}