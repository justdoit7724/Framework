#include "stdafx.h"
#include "MeshLoader.h"
#include "ShaderFormat.h"
#include "Shape.h"
#include "Object.h"
#include "TextureMgr.h"
#include "Shader.h"
#include <mutex>

void ProcessNode(std::vector<Object*>& storage, std::string filepath, aiNode* node, const aiScene* scene)
{
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{

		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		const aiMaterial* const material = scene->mMaterials[mesh->mMaterialIndex];

		assert(mesh->HasNormals());
		assert(mesh->mMaterialIndex >= 0);
		assert(mesh->HasTangentsAndBitangents());
		
		std::vector<Vertex> vertice(mesh->mNumVertices);
		std::vector<UINT> indice(mesh->mNumFaces*3);
		std::string diffMtl = "";
		std::string normalMtl = "";
		
		aiString textureName;
		assert(material->GetTexture(aiTextureType_DIFFUSE, 0, &textureName) == aiReturn_SUCCESS);
		diffMtl = textureName.C_Str();
		assert(material->GetTexture(aiTextureType_HEIGHT, 0, &textureName) == aiReturn_SUCCESS);
		normalMtl = textureName.C_Str();

		for (int i = 0; i < mesh->mNumVertices; ++i)
		{
			vertice[i].pos = XMFLOAT3(
				mesh->mVertices[i].x,
				mesh->mVertices[i].y,
				mesh->mVertices[i].z);
			vertice[i].n = XMFLOAT3(
				mesh->mNormals[i].x,
				mesh->mNormals[i].y,
				mesh->mNormals[i].z);
			vertice[i].tangent = XMFLOAT3(
				mesh->mTangents[i].x,
				mesh->mTangents[i].y,
				mesh->mTangents[i].z);
			vertice[i].tex = XMFLOAT2(
				mesh->mTextureCoords[0][i].x,
				mesh->mTextureCoords[0][i].y);
		}
		for (int i = 0; i < mesh->mNumFaces; ++i)
		{
			aiFace face = mesh->mFaces[i];

			for (int j = 0; j < face.mNumIndices; ++j)
			{
				assert(face.mNumIndices == 3);
				indice[(i * 3) + j] = face.mIndices[j];
			}
		}

		Vertex* vData = vertice.data();
		UINT* iData = indice.data();
		TextureMgr::Instance()->Load(diffMtl, "Data\\Model\\" + filepath + "\\" + diffMtl);
		TextureMgr::Instance()->Load(normalMtl, "Data\\Model\\" + filepath + "\\" + normalMtl);
		
		std::shared_ptr<Shape> curShape = std::make_shared<Shape>(vData, sizeof(Vertex), vertice.size(), iData, indice.size(), D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		storage.push_back(new Object("Mesh",
			curShape,
			TextureMgr::Instance()->Get(diffMtl),
			TextureMgr::Instance()->Get(normalMtl)));
	}

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(storage,filepath, node->mChildren[i], scene);
	}
}

void MeshLoader::Load(std::vector<Object*>& storage, std::string filepath, std::string filename)
{
	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile("Data\\Model\\" + filepath+"\\"+filename,
		aiProcess_MakeLeftHanded|
		aiProcess_FlipUVs|
		aiProcess_FlipWindingOrder|
		aiProcess_CalcTangentSpace);

	assert(
		pScene != nullptr &&
		pScene->HasMaterials() &&
		pScene->HasMeshes());

	ProcessNode(storage, filepath, pScene->mRootNode, pScene);
}

