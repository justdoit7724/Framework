#include "stdafx.h"
#include "AssetMgr.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"
#include "Model.h"
#include "FileCtl.h"
#include "XReader.h"

#include "../SDK/Assimp/include/assimp/postprocess.h"
#include "../SDK/Assimp/include/assimp/Importer.hpp"
#include "../SDK/Assimp/include/assimp/scene.h"

#ifdef _WIN64

#pragma comment(lib, __FILE__"/../../SDK/Assimp/lib_x64/assimp-vc142-mtd.lib")

#else

#pragma comment(lib, __FILE__"/../../SDK/Assimp/lib_win32/assimp-vc142-mtd.lib")

#endif


#define PATH_RESOURCE __FILE__"\\..\\..\\Resources"

#define ASSET_ID_MODEL		"Model"
#define ASSET_ID_TEXTURE	"Texture"
#define ASSET_ID_MATERIAL	"Material"
#define ASSET_ID_VERTEX_SHADER		"VShader"
#define ASSET_ID_PIXEL_SHADER		"PShader"
#define ASSET_ID_HULL_SHADER		"HShader"
#define ASSET_ID_DOMAIN_SHADER		"DShader"
#define ASSET_ID_GEOM_SHADER		"GShader"
#define ASSET_ID_COMPUTE_SHADER		"CShader"
#define ASSET_MAT_TEX_DIFF		"StandardTex"
#define ASSET_MAT_TEX_NORM		"NormalTex"


SET_SINGLETON_CPP(AssetMgr, Init)

void AssetMgr::Init()
{
	
}

void AssetMgr::Load()
{
	std::vector<std::string> fileLists{ PATH_RESOURCE };
	while(fileLists.size())
	{
		const std::string curPath = fileLists.back(); fileLists.pop_back();

		std::string ext;
		if (!FileCtl::IsFile(curPath))
		{
			std::vector<std::string> subList;
			FileCtl::GetAll(curPath, subList);
			for (auto sub : subList)
			{
				fileLists.push_back(sub);
			}
			continue;
		}

		if (!FileCtl::GetExt(curPath, ext) || ext != "x")
		{
			continue;
		}

		XFrame list("DATA", "DATA");
		if (XReader::Read(curPath, &list))
		{
			auto assetKindValue = list.GetChild(XASSETKIND);
			if (assetKindValue.size())
			{
				std::string assetKind = assetKindValue[0]->GetStrings()[0];
				if (assetKind == ASSET_ID_MODEL)
				{
					std::string resource = list.GetChild(XRESOURCE)[0]->GetStrings()[0];
					LoadModel(resource);

					/*
					* decide to just have path to fbx file that aleady have all model info
					* so no need to implement x file parsing code again
					* 
					Model* newModel = new Model;

					std::vector<int> skelParentBoneIndice;
					std::unordered_map<std::string, int> skelNames;
					std::vector<XMMATRIX> skelToParents;
					std::vector<std::vector<XMMATRIX>> skelOffsets;

					std::stack<std::pair<int, XComponent*>> nodeStack;
					nodeStack.push(std::pair<int, XComponent*>(-1, list.GetChild(XFRAME)[0]));
					int curId = 0;
					while (nodeStack.size())
					{
						//get cur info
						int parentIndex = nodeStack.top().first;
						XComponent* curComp = nodeStack.top().second; nodeStack.pop();
						std::string curBoneName = curComp->m_name;

						//Bone info
						skelParentBoneIndice.push_back(parentIndex);
						skelToParents.push_back(XMMATRIX(curComp->GetChild(XFRAMETRANSFORMMATRIX)[0]->GetFloats().data()));
						skelNames.insert(std::pair<std::string, int>(curBoneName, curId));

						//Mesh
						auto meshChildren = curComp->GetChild(XMESH);
						for (auto meshChild : meshChildren)
						{
							//vertex
							std::vector<XMFLOAT3> curVertice;
							std::vector<UINT> curIndice;
							auto verticeRaw = meshChild->GetChild(XMESH_VERT)[0]->GetFloats();
							int vertCount = meshChild->GetChild(XMESH_VERT_COUNT)[0]->GetInts()[0];
							for (int i = 0; i <= vertCount; ++i)
							{
								XMFLOAT3 curVert(
									verticeRaw[i * 3],
									verticeRaw[i * 3 + 1],
									verticeRaw[i * 3 + 2]);

								curVertice.push_back(curVert);
							}
							auto indiceRaw = meshChild->GetChild(XMESH_FACE)[0]->GetFloats();
							int indexCount = meshChild->GetChild(XMESH_FACE_COUNT)[0]->GetInts()[0];
							for (int i = 0; i <= indexCount; ++i)
							{
								XMFLOAT2 curIndex(
									verticeRaw[i * 2],
									verticeRaw[i * 2 + 1]);

								curVertice.push_back(curVert);
							}


							//normal
							auto norms = meshChild->GetChild(XMESHNORMALS)[0]->GetChild(XMESHNORMALS_NORM)[0]->GetFloats();
							int normCount = meshChild->GetChild(XMESHNORMALS)[0]->GetChild(XMESHNORMALS_NORM_COUNT)[0]->GetInts()[0];
							for (int i = 0; i <= normCount; ++i)
							{
								XMFLOAT3 curNorm(
									norms[i * 3],
									norms[i * 3 + 1],
									norms[i * 3 + 2]);
							}

							//texcoord
						}

						//recursive
						auto nodeChildren = curComp->GetChild(XFRAME);
						for (auto node : nodeChildren)
						{
							nodeStack.push(std::pair<int, XComponent*>(curId, node));
						}

						curId++;
					}

					newModel->SetSkeleton(
						std::move(skelParentBoneIndice),
						std::move(skelNames),
						std::move(skelToParents),
						std::move(skelOffsets));

					if (m_model.find(curPath) != m_model.end())
						delete m_model[curPath];
					m_model[curPath] = newModel;*/

				}
				else if (assetKind == ASSET_ID_TEXTURE)
				{
					Texture* newTexture = new Texture;

					newTexture->SetKey(list.GetChild(XRESOURCE)[0]->GetStrings()[0]);

					if (m_tex.find(curPath) != m_tex.end())
						delete m_model[curPath];
					m_tex[curPath] = newTexture;

				}
				else if (assetKind == ASSET_ID_MATERIAL)
				{
					Material* newMaterial = new Material;

					std::string diff;
					auto assets = list.GetChild(XASSET);
					for (int i = 0; i <= assets.size(); ++i)
					{
						if (assets[i]->m_name == ASSET_MAT_TEX_DIFF)
						{
							newMaterial->SetDiff(assets[i]->GetStrings()[0]);
						}
						else if (assets[i]->m_name == ASSET_MAT_TEX_NORM)
						{
							newMaterial->SetNorm(assets[i]->GetStrings()[0]);
						}
					}
				}
				else if (assetKind == ASSET_ID_VERTEX_SHADER)
				{
					VShader* newShader = new VShader;

					newShader->SetShader(list.GetChild(XRESOURCE)[0]->GetStrings()[0]);

					if (m_VShader.find(curPath) != m_VShader.end())
						delete m_VShader[curPath];
					m_VShader[curPath] = newShader;
				}
				else if (assetKind == ASSET_ID_PIXEL_SHADER)
				{
					PShader* newShader = new PShader;

					newShader->SetShader(list.GetChild(XRESOURCE)[0]->GetStrings()[0]);

					if (m_PShader.find(curPath) != m_PShader.end())
						delete m_PShader[curPath];
					m_PShader[curPath] = newShader;
				}
				else if (assetKind == ASSET_ID_HULL_SHADER)
				{

				}
				else if (assetKind == ASSET_ID_DOMAIN_SHADER)
				{

				}
				else if (assetKind == ASSET_ID_GEOM_SHADER)
				{

				}
				else if (assetKind == ASSET_ID_COMPUTE_SHADER)
				{

				}
			}
		}
	
	}

	/*

	LoadVShader(folderVS);
	LoadHShader(folderHS);
	LoadDShader(folderDS);
	LoadGShader(folderGS);
	LoadPShader(folderPS);
	LoadTex(folderTex);
	LoadModel(folderModel);*/
}

VShader* AssetMgr::GetVShader(std::string key)
{
	if(m_VShader.find(key)== m_VShader.end())
		return nullptr;

	return m_VShader[key];
}

HShader* AssetMgr::GetHShader(std::string key)
{
	if (m_HShader.find(key) == m_HShader.end())
		return nullptr;

	return m_HShader[key];
}

DShader* AssetMgr::GetDShader(std::string key)
{
	if (m_DShader.find(key) == m_DShader.end())
		return nullptr;

	return m_DShader[key];
}

GShader* AssetMgr::GetGShader(std::string key)
{
	if (m_GShader.find(key) == m_GShader.end())
		return nullptr;

	return m_GShader[key];
}

PShader* AssetMgr::GetPShader(std::string key)
{
	if (m_PShader.find(key) == m_PShader.end())
		return nullptr;

	return m_PShader[key];
}

Model* AssetMgr::GetModel(std::string key)
{
	if (m_model.find(key) == m_model.end())
		return nullptr;

	return m_model[key];
}

Material* AssetMgr::GetMaterial(std::string key)
{
	if (m_mat.find(key) == m_mat.end())
		return nullptr;

	return m_mat[key];
}

Texture* AssetMgr::GetTexture(std::string key)
{
	if (m_tex.find(key) == m_tex.end())
		return nullptr;

	return m_tex[key];
}

void AssetMgr::Load(std::string subPath)
{
}

/*BOOL AssetMgr::LoadVShader(std::string folder)
{
	for (auto shader : m_VShader[graphicID])
		delete shader.second;
	m_VShader[graphicID].clear();

	std::string path = PATH_RESOURCE;
	path += "\\Shader\\Vertex\\";

	std::vector<std::string> files;
	if (!FileCtl::GetFiles(path, files))
		return FALSE;

	for (auto file : files)
	{
		std::string title;
		if (!FileCtl::GetTitle(file, title))
			continue;

		std::vector<const void*> datas;
		datas.push_back(&file);
		graphic->SetPipelineData(PIP_REG::TRANSFORM_SHADER_VERTEX, datas);

		VShader* vshader = new VShader;
		vshader->SetShader(file);
		m_VShader[graphicID].insert(std::pair<std::string,VShader*>(title, vshader));
	}

	if (m_VShader[graphicID].empty())
		return FALSE;

	return TRUE;
}
BOOL AssetMgr::LoadHShader(int graphicID, std::string folder)
{
	for (auto shader : m_HShader[graphicID])
		delete shader.second;
	m_HShader[graphicID].clear();

	std::string path = PATH_RESOURCE;
	path += "\\Shader\\Hull\\";

	std::vector<std::string> files;
	if (!FileCtl::GetFiles(path, files))
		return FALSE;

	for (auto file : files)
	{
		std::string title;
		if (!FileCtl::GetTitle(file, title))
			continue;

		std::vector<const void*> datas;
		datas.push_back(&file);
		graphic->SetPipelineData(PIP_REG::TRANSFORM_SHADER_HULL, datas);

		HShader* hshader = new HShader;
		hshader->SetShader(file);
		m_HShader[graphicID].insert(std::pair<std::string, HShader*>(title, hshader));
	}

	if (m_HShader[graphicID].empty())
		return FALSE;

	return TRUE;
}
BOOL AssetMgr::LoadDShader(int graphicID, std::string folder)
{
	for (auto shader : m_DShader[graphicID])
		delete shader.second;
	m_DShader[graphicID].clear();

	std::string path = PATH_RESOURCE;
	path += "\\Shader\\Domain\\";

	std::vector<std::string> files;
	if (!FileCtl::GetFiles(path, files))
		return FALSE;

	for (auto file : files)
	{
		std::string title;
		if (!FileCtl::GetTitle(file, title))
			continue;

		std::vector<const void*> datas;
		datas.push_back(&file);
		graphic->SetPipelineData(PIP_REG::TRANSFORM_SHADER_DOMAIN, datas);

		DShader* dshader = new DShader;
		dshader->SetShader(file);
		m_DShader[graphicID].insert(std::pair<std::string, DShader*>(title, dshader));
	}

	if (m_DShader[graphicID].empty())
		return FALSE;

	return TRUE;
}
BOOL AssetMgr::LoadGShader(int graphicID, std::string folder)
{
	for (auto shader : m_GShader[graphicID])
		delete shader.second;
	m_GShader[graphicID].clear();

	std::string path = PATH_RESOURCE;
	path += "\\Shader\\Geometry\\";

	std::vector<std::string> files;
	if (!FileCtl::GetFiles(path, files))
		return FALSE;

	for (auto file : files)
	{
		std::string title;
		if (!FileCtl::GetTitle(file, title))
			continue;

		std::vector<const void*> datas;
		datas.push_back(&file);
		graphic->SetPipelineData(PIP_REG::TRANSFORM_SHADER_GEOMETRY, datas);

		GShader* gshader = new GShader;
		gshader->SetShader(file);
		m_GShader[graphicID].insert(std::pair<std::string, GShader*>(title, gshader));
	}

	if (m_GShader[graphicID].empty())
		return FALSE;

	return TRUE;
}
BOOL AssetMgr::LoadPShader(int graphicID, std::string folder)
{
	for (auto shader : m_PShader[graphicID])
		delete shader.second;
	m_PShader[graphicID].clear();

	std::string path = PATH_RESOURCE;
	path += "\\Shader\\Pixel\\";

	std::vector<std::string> files;
	if (!FileCtl::GetFiles(path, files))
		return FALSE;

	for (auto file : files)
	{
		std::string title;
		if (!FileCtl::GetTitle(file, title))
			continue;

		std::vector<const void*> datas;
		datas.push_back(&file);
		graphic->SetPipelineData(PIP_REG::PIXEL_SHADER_PIXEL, datas);

		PShader* pshader = new PShader;
		pshader->SetShader(file);
		m_PShader[graphicID].insert(std::pair<std::string, PShader*>(title, pshader));
	}

	if (m_PShader[graphicID].empty())
		return FALSE;

	return TRUE;
}
BOOL AssetMgr::LoadTex(int graphicID, std::string folder)
{
	for (auto tex : m_tex[graphicID])
		delete tex.second;
	m_tex[graphicID].clear();

	std::vector<std::string> files;
	if (!FileCtl::GetFiles(folder, files))
		return FALSE;

	for (auto file : files)
	{
		std::string title;
		if (!FileCtl::GetTitle(file, title))
			continue;
		
		Texture* texture = new Texture;
		texture->SetKey(title);
		m_tex[graphicID].insert(std::pair<std::string, Texture*>(title, texture));

		std::vector<const void*> datas;
		datas.push_back(&file);
		graphic->SetPipelineData(PIP_REG::PIXEL_TEX, datas);
	}

	if (m_tex[graphicID].empty())
		return FALSE;

	return TRUE;
}*/
BOOL AssetMgr::LoadModel(std::string folder)
{
	ReleaseModel();

	std::string title;
	std::string ext;
	if (!FileCtl::GetTitle(folder, title) || !FileCtl::GetExt(title, ext) || ext!="fbx")
		return FALSE;

	Assimp::Importer importer;
	const aiScene* pScene = importer.ReadFile(folder,
		aiProcess_MakeLeftHanded |
		aiProcess_FlipWindingOrder |
		aiProcess_FlipUVs |
		aiProcess_Triangulate);

	Model* newModel = new Model;

	std::vector<XMMATRIX> vmatToParent;
	std::vector<int> viHierarchy;
	std::vector<std::vector<XMMATRIX>> vmatOffset;
	std::unordered_map<std::string, int> mNodeName;
	std::vector<BOOL> vBoneNode;

	std::stack<std::pair<int, aiNode*>> stNode;
	stNode.push(std::pair<int, aiNode*>(-1, pScene->mRootNode));
	int curId = 0;
	while (!stNode.empty())
	{
		const int parentId = stNode.top().first;
		aiNode* curNode = stNode.top().second;
		stNode.pop();

		for (int i = curNode->mNumChildren - 1; i >= 0; --i)
		{
			stNode.push(std::pair<int, aiNode*>(curId, curNode->mChildren[i]));
		}

		auto matToParent = curNode->mTransformation;
		XMMATRIX xmmatToParent(
			matToParent.a1, matToParent.b1, matToParent.c1, matToParent.d1,
			matToParent.a2, matToParent.b2, matToParent.c2, matToParent.d2,
			matToParent.a3, matToParent.b3, matToParent.c3, matToParent.d3,
			matToParent.a4, matToParent.b4, matToParent.c4, matToParent.d4);
		vmatToParent.push_back(xmmatToParent);
		viHierarchy.push_back(parentId);
		mNodeName.insert(std::pair<std::string, int>(curNode->mName.C_Str(), curId));
		curId++;
	}
	vBoneNode.resize(curId);
	vmatOffset.resize(curId);

	for (int i = 0; i < pScene->mNumMeshes; ++i)
	{
		aiMesh* curMesh = pScene->mMeshes[i];

		std::vector<DX::Vertex> vVertice;
		std::vector<UINT> vIndice;
		for (int j = 0; j < curMesh->mNumVertices; ++j)
		{
			auto vertex = curMesh->mVertices[j];
			auto normal = curMesh->mNormals[j];
			auto tex =  curMesh->mTextureCoords[0][j];
			DX::Vertex newVertex;
			newVertex.pos = XMFLOAT3(vertex.x, vertex.y, vertex.z);
			newVertex.n = XMFLOAT3(normal.x, normal.y, normal.z);
			newVertex.tex = XMFLOAT2(tex.x, tex.y);
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

		vmatOffset[i].resize(viHierarchy.size());
		for (int j = 0; j < curMesh->mNumBones; ++j)
		{
			aiBone* curBone = curMesh->mBones[j];
			std::string strName = curBone->mName.C_Str();
			if (mNodeName.find(strName) == mNodeName.end())
				continue;
			curId = mNodeName[strName];
			vBoneNode[curId] = TRUE;

			for (int k = 0; k < curBone->mNumWeights; ++k)
			{
				auto curWeight = curBone->mWeights[k];
				for (int l = 0; l < 4; ++l)
				{
					if (vVertice[curWeight.mVertexId].boneId[l] == -1)
					{
						vVertice[curWeight.mVertexId].boneId[l] = curId;
						break;
					}
				}
			}
			auto matOffset = curBone->mOffsetMatrix;
			XMMATRIX xmmatOffset(
				matOffset.a1, matOffset.b1, matOffset.c1, matOffset.d1,
				matOffset.a2, matOffset.b2, matOffset.c2, matOffset.d2,
				matOffset.a3, matOffset.b3, matOffset.c3, matOffset.d3,
				matOffset.a4, matOffset.b4, matOffset.c4, matOffset.d4);
			vmatOffset[i][curId] = xmmatOffset;
		}

		newModel->AddSubMesh(i, vVertice, vIndice);
	}

	newModel->SetSkeleton(std::move(viHierarchy), std::move(mNodeName), std::move(vmatToParent), std::move(vmatOffset));

	//if (pScene->HasMaterials())
	//{
	//	for (int i = 0; i < pScene->mNumMaterials; ++i)
	//	{
	//		aiMaterial* material = pScene->mMaterials[i];
	//		for (int j = 0; j < material->mNumProperties; ++j)
	//		{
	//			aiMaterialProperty* matProperty = material->mProperties[j];
	//
	//			aiString key = matProperty->mKey;
	//		}
	//	}
	//
	//	//SGL_TEX.Load(device, dContext, )
	//
	//	//auto pMaterial = new Material();
	//}
	

	
	if (pScene->HasAnimations())
	{
		for (int i = 0; i < pScene->mNumAnimations; ++i) // animation count
		{
			AnimationClip newClip;
			newClip.boneAnims.resize(viHierarchy.size());

			auto curAnim = pScene->mAnimations[i];
			for (int j = 0; j < curAnim->mNumChannels; ++j) // bones's info
			{
				auto curAnimChannel = curAnim->mChannels[j];
				int iCurId = -1;
				std::string boneName = curAnimChannel->mNodeName.C_Str();
				if (mNodeName.find(boneName) == mNodeName.end())
					continue;
				iCurId = mNodeName[boneName];
	
				BoneAnimation animFrame;
				assert(
					curAnimChannel->mNumPositionKeys == curAnimChannel->mNumRotationKeys &&
					curAnimChannel->mNumPositionKeys == curAnimChannel->mNumScalingKeys);
				for (int k = 0; k < curAnimChannel->mNumPositionKeys; ++k)
				{
					aiVector3D curPosKey = curAnimChannel->mPositionKeys[k].mValue;
					aiQuaternion curRotKey = curAnimChannel->mRotationKeys[k].mValue;
					aiVector3D curScaleKey = curAnimChannel->mScalingKeys[k].mValue;
	
					KeyFrame newFrame;
					newFrame.timePos = curAnimChannel->mPositionKeys[k].mTime;
					newFrame.translation = XMFLOAT3(curPosKey.x, curPosKey.y, curPosKey.z);
					newFrame.rotationQuat = XMFLOAT4(curRotKey.x, curRotKey.y, curRotKey.z, curRotKey.w);
					newFrame.scale = XMFLOAT3(curScaleKey.x, curScaleKey.y, curScaleKey.z);
					animFrame.keyFrames.push_back(newFrame);
				}
	
				newClip.boneAnims[iCurId] = animFrame;
			}
	
			newModel->SetAnimations(curAnim->mName.C_Str(), newClip);
		}
	}
	
	m_model[folder] = newModel;
	
	return TRUE;
}

void AssetMgr::ReleaseModel()
{
	for (auto mesh : m_model)
	{
		delete mesh.second;
	}
	m_model.clear();
}

void AssetMgr::ReleaseShader()
{
	for (auto shaders : m_VShader)
		delete shaders.second;
	for (auto shaders : m_HShader)
		delete shaders.second;
	for (auto shaders : m_DShader)
		delete shaders.second;
	for (auto shaders : m_GShader)
		delete shaders.second;
	for (auto shaders : m_PShader)
		delete shaders.second;
	m_VShader.clear();
	m_HShader.clear();
	m_DShader.clear();
	m_GShader.clear();
	m_PShader.clear();
}

void AssetMgr::ReleaseTexture()
{
	for (auto tex : m_tex)
	{
		delete tex.second;
	}
	m_tex.clear();
}

void AssetMgr::ReleaseMaterial()
{
	for (auto mat : m_mat)
	{
		delete mat.second;
	}
	m_mat.clear();
}

