
#include "pch.h"

#include "MeshLoader.h"
#include "ShaderFormat.h"
#include "Object.h"
#include "Shader.h"
#include "Mesh.h"
#include "Animation.h"
#include <mutex>

#include "_Packages\Assimp.3.0.0\include\postprocess.h"
#include "_Packages\Assimp.3.0.0\include\Importer.hpp"
#include "_Packages\Assimp.3.0.0\include\scene.h"

namespace DX
{
	void LoadOBJ(ID3D11Device* device, SkinnedData* storage, std::string filepath, std::string filename)
	{
		Assimp::Importer importer;

		const aiScene* pScene = importer.ReadFile(filepath + "\\" + filename,
			aiProcess_MakeLeftHanded |
			aiProcess_FlipUVs |
			aiProcess_FlipWindingOrder |
			aiProcess_GenNormals|
			aiProcess_Triangulate);

		std::vector<Mesh*> vMesh;


		std::queue<aiNode*> qNode;
		qNode.push(pScene->mRootNode);

		bool hasAnim = pScene->HasAnimations();

		while (!qNode.empty())
		{
			aiNode* curNode = qNode.front(); qNode.pop();

			int iMeshID = curNode->mMeshes[0];
			aiMesh* mesh = pScene->mMeshes[iMeshID];

			std::vector<Vertex> vertice;
			std::vector<UINT> indice;
			for (int j = 0; j < mesh->mNumVertices; ++j)
			{
				Vertex vertex;
				ZeroMemory(&vertex, sizeof(Vertex));
				vertex.pos = XMFLOAT3(
					mesh->mVertices[j].x,
					mesh->mVertices[j].y,
					mesh->mVertices[j].z);
				vertex.n = XMFLOAT3(
					mesh->mNormals[j].x,
					mesh->mNormals[j].y,
					mesh->mNormals[j].z);
				vertice.push_back(vertex);
			}
			for (int i = 0; i < mesh->mNumFaces; ++i)
			{
				aiFace face = mesh->mFaces[i];

				for (int j = 0; j < face.mNumIndices; ++j)
				{
					indice.push_back(face.mIndices[j]);
				}
			}

			for (int i = 0; i < curNode->mNumChildren; ++i)
			{
				qNode.push(curNode->mChildren[i]);
			}
		}
	}
}