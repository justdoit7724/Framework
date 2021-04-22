#include "stdafx.h"
#include <filesystem>
#include "ResourceMgr.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"
#include "Skeleton.h"
#include "Animation.h"


#include "../Packages/Assimp/include/assimp/postprocess.h"
#include "../Packages/Assimp/include/assimp/Importer.hpp"
#include "../Packages/Assimp/include/assimp/scene.h"

#ifdef _WIN64

#pragma comment(lib, __FILE__"/../../Packages/Assimp/lib_x64/assimp-vc142-mtd.lib")

#else

#pragma comment(lib, __FILE__"/../../Packages/Assimp/lib_win32/assimp-vc142-mtd.lib")

#endif


#define PATH_RESOURCE __FILE__"\\..\\..\\Resources"

BOOL FindTitle(std::string str, std::string& title)
{
	title = "";

	std::string tmpPath = str;
	int firstPos = -1;
	int lastPos = -1;
	for (int i = tmpPath.size() - 1; i >= 0; --i)
	{
		if (tmpPath[i] == '\\' || tmpPath[i] == '/')
		{
			firstPos = i + 1;
			break;
		}
		else if (tmpPath[i] == '.')
		{
			lastPos = i - 1;
		}
	}
	if (firstPos == -1 || lastPos == -1 || firstPos >= lastPos)
		return FALSE;

	title = tmpPath.substr(firstPos, lastPos - firstPos + 1);
	if (title == "")
		return FALSE;

	return TRUE;
}

SET_SINGLETON_CPP(ResourceMgr, Init)

void ResourceMgr::Init()
{
	
}

void ResourceMgr::Load(DX::Graphic* graphic)
{
	std::string folderVS = PATH_RESOURCE;
	folderVS += "\\Shader\\Vertex";
	std::string folderPS = PATH_RESOURCE;
	folderPS += "\\Shader\\Pixel";
	std::string folderHS = PATH_RESOURCE;
	folderHS += "\\Shader\\Hull";
	std::string folderDS = PATH_RESOURCE;
	folderDS += "\\Shader\\Domain";
	std::string folderGS = PATH_RESOURCE;
	folderGS += "\\Shader\\Geometry";
	std::string folderTex = PATH_RESOURCE;
	folderTex += "\\Shader\\Texture";
	std::string folderModel = PATH_RESOURCE;
	folderModel += "\\Shader\\Model";

	LoadVShader(graphic, folderVS);
	LoadHShader(graphic, folderHS);
	LoadDShader(graphic, folderDS);
	LoadGShader(graphic, folderGS);
	LoadPShader(graphic, folderPS);
	LoadTex(graphic, folderTex);
	LoadModel(folderModel);
}

VShader* ResourceMgr::GetVShader(DX::Graphic* graphic, std::string key)
{
	if(m_VShader.find(graphic->m_id)== m_VShader.end() && m_VShader[graphic->m_id].find(key)== m_VShader[graphic->m_id].end())
		return nullptr;

	return m_VShader[graphic->m_id][key];
}

HShader* ResourceMgr::GetHShader(DX::Graphic* graphic, std::string key)
{
	if (m_HShader.find(graphic->m_id) == m_HShader.end() && m_HShader[graphic->m_id].find(key) == m_HShader[graphic->m_id].end())
		return nullptr;

	return m_HShader[graphic->m_id][key];
}

DShader* ResourceMgr::GetDShader(DX::Graphic* graphic, std::string key)
{
	if (m_DShader.find(graphic->m_id) == m_DShader.end() && m_DShader[graphic->m_id].find(key) == m_DShader[graphic->m_id].end())
		return nullptr;

	return m_DShader[graphic->m_id][key];
}

GShader* ResourceMgr::GetGShader(DX::Graphic* graphic, std::string key)
{
	if (m_GShader.find(graphic->m_id) == m_GShader.end() && m_GShader[graphic->m_id].find(key) == m_GShader[graphic->m_id].end())
		return nullptr;

	return m_GShader[graphic->m_id][key];
}

PShader* ResourceMgr::GetPShader(DX::Graphic* graphic, std::string key)
{
	if (m_PShader.find(graphic->m_id) == m_PShader.end() && m_PShader[graphic->m_id].find(key) == m_PShader[graphic->m_id].end())
		return nullptr;

	return m_PShader[graphic->m_id][key];
}

Mesh* ResourceMgr::GetMesh(std::string key)
{
	if (m_mesh.find(key) == m_mesh.end())
		return nullptr;

	return m_mesh[key];
}

Material* ResourceMgr::GetMaterial(DX::Graphic* graphic, std::string key)
{
	if (m_mat.find(graphic->m_id) == m_mat.end() && m_mat[graphic->m_id].find(key) == m_mat[graphic->m_id].end())
		return nullptr;

	return m_mat[graphic->m_id][key];
}

Texture* ResourceMgr::GetTexture(DX::Graphic* graphic, std::string key)
{
	if (m_tex.find(graphic->m_id) == m_tex.end() && m_tex[graphic->m_id].find(key) == m_tex[graphic->m_id].end())
		return nullptr;

	return m_tex[graphic->m_id][key];
}

BOOL ResourceMgr::AddMaterial(DX::Graphic* graphic, std::string key, Material* mat)
{
	if (m_mat[graphic->m_id].find(key) != m_mat[graphic->m_id].end())
		return FALSE;

	m_mat[graphic->m_id][key] = mat;

	return TRUE;
}


BOOL ResourceMgr::LoadVShader(DX::Graphic* graphic, std::string folder)
{
	for (auto shader : m_VShader[graphic->m_id])
		delete shader.second;
	m_VShader[graphic->m_id].clear();

	std::string path = PATH_RESOURCE;
	path += "\\Shader\\Vertex\\";

	for (const auto& file : std::filesystem::directory_iterator(path))
	{
		std::wstring totalPathW = file.path().c_str();
		std::string totalPathA = std::string(totalPathW.begin(), totalPathW.end());
		std::string title;
		if (!FindTitle(totalPathA, title))
			continue;

		std::vector<const void*> datas;
		datas.push_back(&title);
		datas.push_back(&totalPathA);
		graphic->SetPipelineData(PIP_REG::TRANSFORM_SHADER_VERTEX, datas);

		VShader* vshader = new VShader;
		vshader->SetShader(title);
		m_VShader[graphic->m_id].insert(std::pair<std::string,VShader*>(title, vshader));
	}

	if (m_VShader[graphic->m_id].empty())
		return FALSE;

	return TRUE;
}
BOOL ResourceMgr::LoadHShader(DX::Graphic* graphic, std::string folder)
{
	for (auto shader : m_HShader[graphic->m_id])
		delete shader.second;
	m_HShader[graphic->m_id].clear();

	std::string path = PATH_RESOURCE;
	path += "\\Shader\\Hull\\";

	for (const auto& file : std::filesystem::directory_iterator(path))
	{
		std::wstring totalPathW = file.path().c_str();
		std::string totalPathA = std::string(totalPathW.begin(), totalPathW.end());
		std::string title;
		if (!FindTitle(totalPathA, title))
			continue;

		std::vector<const void*> datas;
		datas.push_back(&title);
		datas.push_back(&totalPathA);
		graphic->SetPipelineData(PIP_REG::TRANSFORM_SHADER_HULL, datas);

		HShader* hshader = new HShader;
		hshader->SetShader(title);
		m_HShader[graphic->m_id].insert(std::pair<std::string, HShader*>(title, hshader));
	}

	if (m_HShader[graphic->m_id].empty())
		return FALSE;

	return TRUE;
}
BOOL ResourceMgr::LoadDShader(DX::Graphic* graphic, std::string folder)
{
	for (auto shader : m_DShader[graphic->m_id])
		delete shader.second;
	m_DShader[graphic->m_id].clear();

	std::string path = PATH_RESOURCE;
	path += "\\Shader\\Domain\\";

	for (const auto& file : std::filesystem::directory_iterator(path))
	{
		std::wstring totalPathW = file.path().c_str();
		std::string totalPathA = std::string(totalPathW.begin(), totalPathW.end());
		std::string title;
		if (!FindTitle(totalPathA, title))
			continue;

		std::vector<const void*> datas;
		datas.push_back(&title);
		datas.push_back(&totalPathA);
		graphic->SetPipelineData(PIP_REG::TRANSFORM_SHADER_DOMAIN, datas);

		DShader* dshader = new DShader;
		dshader->SetShader(title);
		m_DShader[graphic->m_id].insert(std::pair<std::string, DShader*>(title, dshader));
	}

	if (m_DShader[graphic->m_id].empty())
		return FALSE;

	return TRUE;
}
BOOL ResourceMgr::LoadGShader(DX::Graphic* graphic, std::string folder)
{
	for (auto shader : m_GShader[graphic->m_id])
		delete shader.second;
	m_GShader[graphic->m_id].clear();

	std::string path = PATH_RESOURCE;
	path += "\\Shader\\Geometry\\";

	for (const auto& file : std::filesystem::directory_iterator(path))
	{
		std::wstring totalPathW = file.path().c_str();
		std::string totalPathA = std::string(totalPathW.begin(), totalPathW.end());
		std::string title;
		if (!FindTitle(totalPathA, title))
			continue;

		std::vector<const void*> datas;
		datas.push_back(&title);
		datas.push_back(&totalPathA);
		graphic->SetPipelineData(PIP_REG::TRANSFORM_SHADER_GEOMETRY, datas);

		GShader* gshader = new GShader;
		gshader->SetShader(title);
		m_GShader[graphic->m_id].insert(std::pair<std::string, GShader*>(title, gshader));
	}

	if (m_GShader[graphic->m_id].empty())
		return FALSE;

	return TRUE;
}
BOOL ResourceMgr::LoadPShader(DX::Graphic* graphic, std::string folder)
{
	for (auto shader : m_PShader[graphic->m_id])
		delete shader.second;
	m_PShader[graphic->m_id].clear();

	std::string path = PATH_RESOURCE;
	path += "\\Shader\\Pixel\\";

	for (const auto& file : std::filesystem::directory_iterator(path))
	{
		std::wstring totalPathW = file.path().c_str();
		std::string totalPathA = std::string(totalPathW.begin(), totalPathW.end());
		std::string title;
		if (!FindTitle(totalPathA, title))
			continue;

		std::vector<const void*> datas;
		datas.push_back(&title);
		datas.push_back(&totalPathA);
		graphic->SetPipelineData(PIP_REG::PIXEL_SHADER_PIXEL, datas);

		PShader* pshader = new PShader;
		pshader->SetShader(title);
		m_PShader[graphic->m_id].insert(std::pair<std::string, PShader*>(title, pshader));
	}

	if (m_PShader[graphic->m_id].empty())
		return FALSE;

	return TRUE;
}
BOOL ResourceMgr::LoadTex(DX::Graphic* graphic, std::string folder)
{
	for (auto tex : m_tex[graphic->m_id])
		delete tex.second;
	m_tex[graphic->m_id].clear();

	for (const auto& file : std::filesystem::directory_iterator(folder))
	{
		std::wstring totalPathW = file.path().c_str();
		std::string totalPathA = std::string(totalPathW.begin(), totalPathW.end());
		std::string title;
		if (!FindTitle(totalPathA, title))
			continue;
		
		Texture* texture = new Texture;
		texture->SetKey(title);
		m_tex[graphic->m_id].insert(std::pair<std::string, Texture*>(title, texture));

		std::vector<const void*> datas;
		datas.push_back(&totalPathA);
		graphic->SetPipelineData(PIP_REG::PIXEL_TEX, datas);
	}

	if (m_tex[graphic->m_id].empty())
		return FALSE;

	return TRUE;
}
BOOL ResourceMgr::LoadModel(std::string folder)
{
	ReleaseModel();

	std::string path = PATH_RESOURCE;
	path += "\\Model\\";

	for (const auto& file : std::filesystem::directory_iterator(path))
	{
		std::wstring totalPathW = file.path().c_str();
		std::string totalPathA = std::string(totalPathW.begin(), totalPathW.end());
		std::string title;
		if (!FindTitle(totalPathA, title))
			continue;

		Mesh* newMesh = new Mesh;
		Skeleton* newSkel = new Skeleton;

		Assimp::Importer importer;
		const aiScene* pScene = importer.ReadFile(totalPathA,
			aiProcess_MakeLeftHanded |
			aiProcess_FlipWindingOrder |
			aiProcess_FlipUVs |
			aiProcess_Triangulate);

		std::stack<std::pair<int, aiNode*>> stNode;
		stNode.push(std::pair<int, aiNode*>(-1, pScene->mRootNode));

		std::vector<XMMATRIX> vmatToParent;
		std::vector<int> viHierarchy;
		std::vector<XMMATRIX> vmatOffset;
		std::unordered_map<std::string, int> mNameHierarchy;
		std::vector<aiMesh*> vMesh;

		while (!stNode.empty())
		{
			int curId = stNode.top().first;
			aiNode* curNode = stNode.top().second;
			stNode.pop();

			for (int i = curNode->mNumChildren - 1; i >= 0; --i)
			{
				stNode.push(std::pair<int, aiNode*>(viHierarchy.size(), curNode->mChildren[i]));
			}

			auto matToParent = curNode->mTransformation;
			XMMATRIX xmmatToParent(
				matToParent.a1, matToParent.b1, matToParent.c1, matToParent.d1,
				matToParent.a2, matToParent.b2, matToParent.c2, matToParent.d2,
				matToParent.a3, matToParent.b3, matToParent.c3, matToParent.d3,
				matToParent.a4, matToParent.b4, matToParent.c4, matToParent.d4);
			vmatToParent.push_back(xmmatToParent);
			viHierarchy.push_back(curId);
			mNameHierarchy.insert(std::pair<std::string, int>(curNode->mName.C_Str(), mNameHierarchy.size() - 1));
		}
		vmatOffset.resize(mNameHierarchy.size());

		for (int i = 0; i < pScene->mNumMeshes; ++i)
		{
			aiMesh* curMesh = pScene->mMeshes[i];
			vMesh.push_back(curMesh);

			std::vector<DX::Vertex> vVertice;
			std::vector<UINT> vIndice;
			for (int j = 0; j < curMesh->mNumVertices; ++j)
			{
				auto vertex = curMesh->mVertices[j];
				auto normal = curMesh->mNormals[j];
				DX::Vertex newVertex;
				ZeroMemory(&newVertex, sizeof(DX::Vertex));
				newVertex.pos = XMFLOAT3(vertex.x, vertex.y, vertex.z);
				newVertex.n = XMFLOAT3(normal.x, normal.y, normal.z);
				vVertice.push_back(newVertex);
			}
			for (int i = 0; i < curMesh->mNumFaces; ++i)
			{
				aiFace face = curMesh->mFaces[i];
				assert(face.mNumIndices == 3);

				for (int j = 0; j < face.mNumIndices; ++j)
				{
					vIndice.push_back(face.mIndices[j]);
				}
			}


			std::vector<DX::Vertex> vertice;
			std::vector<UINT> indice;
			for (int j = 0; j < curMesh->mNumBones; ++j)
			{
				aiBone* curBone = curMesh->mBones[j];

				std::string strName = curBone->mName.C_Str();
				assert(mNameHierarchy.find(strName) != mNameHierarchy.end());
				int iParentId = mNameHierarchy.at(strName);
				int iCurId = iParentId + 1;

				for (int k = 0; k < curBone->mNumWeights; ++k)
				{
					auto curWeight = curBone->mWeights[k];
					vVertice[curWeight.mVertexId].boneId = iCurId;
				}
				auto matOffset = curBone->mOffsetMatrix;
				XMMATRIX xmmatOffset(
					matOffset.a1, matOffset.b1, matOffset.c1, matOffset.d1,
					matOffset.a2, matOffset.b2, matOffset.c2, matOffset.d2,
					matOffset.a3, matOffset.b3, matOffset.c3, matOffset.d3,
					matOffset.a4, matOffset.b4, matOffset.c4, matOffset.d4);
				vmatOffset[iCurId] = xmmatOffset;
			}

			std::string keySubMesh = title + "_" + std::to_string(i);
			SubMesh* subMesh = new SubMesh(keySubMesh,vVertice, vIndice);
			newMesh->SetSubMesh(i, subMesh);
		}

		newSkel->Set(viHierarchy, mNameHierarchy, vmatToParent, vmatOffset);

		/*
		if (pScene->HasMaterials())
		{
			for (int i = 0; i < pScene->mNumMaterials; ++i)
			{
				aiMaterial* material = pScene->mMaterials[i];
				for (int j = 0; j < material->mNumProperties; ++j)
				{
					aiMaterialProperty* matProperty = material->mProperties[j];

					aiString key = matProperty->mKey;
				}
			}

			//SGL_TEX.Load(device, dContext, )

			//auto pMaterial = new Material();
		}
		*/

		/*
		if (pScene->HasAnimations())
		{
			auto pSkeleton = new Skeleton(viHierarchy, mNameHierarchy, vmatToParent, vmatOffset);
			auto pAnim = new Animation();

			for (int i = 0; i < pScene->mNumAnimations; ++i)
			{
				AnimationClip newClip;
				newClip.BoneAnimations.resize(viHierarchy.size());
				auto curAnim = pScene->mAnimations[i];
				for (int j = 0; j < curAnim->mNumChannels; ++j)
				{
					auto curAnimChannel = curAnim->mChannels[j];
					std::string strNodeName = curAnimChannel->mNodeName.C_Str();
					int iCurId = mNameHierarchy.at(strNodeName) + 1;

					BoneAnimation animFrame;
					assert(
						curAnimChannel->mNumPositionKeys == curAnimChannel->mNumRotationKeys &&
						curAnimChannel->mNumPositionKeys == curAnimChannel->mNumScalingKeys);
					for (int k = 0; k < curAnimChannel->mNumPositionKeys; ++k)
					{
						KeyFrame newFrame;
						aiVector3D curPosKey = curAnimChannel->mPositionKeys[k].mValue;
						aiQuaternion curRotKey = curAnimChannel->mRotationKeys[k].mValue;
						aiVector3D curScaleKey = curAnimChannel->mScalingKeys[k].mValue;

						//newFrame.timePos = curAnimChannel->
						newFrame.timePos = curAnimChannel->mPositionKeys[k].mTime;
						newFrame.translation = XMFLOAT3(curPosKey.x, curPosKey.y, curPosKey.z);
						newFrame.rotationQuat = XMFLOAT4(curRotKey.x, curRotKey.y, curRotKey.z, curRotKey.w);
						newFrame.scale = XMFLOAT3(curScaleKey.x, curScaleKey.y, curScaleKey.z);
						animFrame.Keyframes.push_back(newFrame);
					}

					newClip.BoneAnimations[iCurId] = animFrame;
				}

				std::string strAnimName = curAnim->mName.C_Str();

				pAnim->Set(strAnimName, newClip);
			}

			(*pMesh)->SetSkeleton(pSkeleton);
			(*pMesh)->SetAnimation(pAnim);
		}
		*/



		m_mesh[title] = newMesh;
		m_skel[title] = newSkel;
	}

	if (m_mesh.empty())
		return FALSE;

	return TRUE;
}

void ResourceMgr::ReleaseModel()
{
	for (auto mesh : m_mesh)
	{
		delete mesh.second;
	}
	m_mesh.clear();
}

void ResourceMgr::ReleaseShader()
{
	for (auto shaders : m_VShader)
		for (auto shader : shaders.second)
			delete shader.second;
	for (auto shaders : m_HShader)
		for (auto shader : shaders.second)
			delete shader.second;
	for (auto shaders : m_DShader)
		for (auto shader : shaders.second)
			delete shader.second;
	for (auto shaders : m_GShader)
		for (auto shader : shaders.second)
			delete shader.second;
	for (auto shaders : m_PShader)
		for (auto shader : shaders.second)
			delete shader.second;
	m_VShader.clear();
	m_HShader.clear();
	m_DShader.clear();
	m_GShader.clear();
	m_PShader.clear();
}

void ResourceMgr::ReleaseTexture()
{
	for (auto mesh : m_mesh)
	{
		delete mesh.second;
	}
	m_mesh.clear();
}

void ResourceMgr::ReleaseMaterial()
{
	for (auto mesh : m_mesh)
	{
		delete mesh.second;
	}
	m_mesh.clear();
}

