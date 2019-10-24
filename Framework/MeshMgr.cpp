#include "MeshMgr.h"
#include "ShaderFormat.h"
#include <mutex>

void MeshMgr::Add(std::string key, std::string filepath)
{
	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile("Data\\Model\\"+filepath,
		aiProcess_Triangulate);

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
		assert(mesh->HasNormals());
		
		meshes[key].push_back(ProcessMesh(mesh, scene->mMaterials));
	}

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		this->ProcessNode(key, node->mChildren[i], scene);
	}
}

MeshInfo MeshMgr::ProcessMesh(aiMesh* mesh, const aiMaterial*const* materials)
{

	MeshInfo meshInfo;

	assert(mesh->mMaterialIndex >= 0);
	const aiMaterial*const material = materials[mesh->mMaterialIndex];
	assert(material->GetTexture(aiTextureType_DIFFUSE, 0, &meshInfo.diffKey) == aiReturn_SUCCESS);
	assert(material->GetTexture(aiTextureType_HEIGHT, 0, &meshInfo.normalKey) == aiReturn_SUCCESS);
	material->GetTexture(aiTextureType_SPECULAR, 0, &meshInfo.specKey);

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

		meshInfo.vertice[i] = curVertex;
	}
	for (int i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];
		
		for (int j = 0; j < face.mNumIndices; ++j)
		{
			assert(face.mNumIndices == 3);
			meshInfo.indice[i] = face.mIndices[j];
		}
	}

	return meshInfo;
}
