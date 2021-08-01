#include "stdafx.h"
#include "Model.h"

Model::~Model()
{
}

void Model::AddSubMesh(int id, const std::vector<DX::Vertex>& vertice, const std::vector<UINT>& indice)
{
	if (id >= m_subMeshes.size())
	{
		m_subMeshes.resize(id + 1);
	}

	m_subMeshes[id].vertice.clear();
	for (int i=0; i< vertice.size(); ++i)
	{
		m_subMeshes[id].vertice.push_back(vertice[i]);
	}
	m_subMeshes[id].indice.clear();
	for (auto index : indice)
	{
		m_subMeshes[id].indice.push_back(index);
	}
}

void Model::SetSkeleton(const std::vector<int>&& parentIndice, const std::unordered_map<std::string,int>&& names, const std::vector<XMMATRIX>&& toParents, const std::vector<std::vector<XMMATRIX>>&& offsets)
{
	m_SkelParentIndice = parentIndice;
	m_SkelNames = names;
	m_SkelToParents = toParents;
	m_SkelOffset = offsets;
}

void Model::SetAnimations(std::string name, AnimationClip clip)
{
	m_animClips[name] = clip;
}

std::vector<XMMATRIX> Model::GetFinalTransform(int id, std::string name, float t)
{
	if (m_animClips.find(name) == m_animClips.end())
		return std::vector<XMMATRIX>();

	std::vector<XMMATRIX> finalTransforms;

	int nBone = m_SkelToParents.size();

	std::vector<XMMATRIX> lerpToParents;
	const AnimationClip& clip = m_animClips.find(name)->second;
	clip.Interpolate(t, lerpToParents);

	std::vector<XMMATRIX> lerpToParentsWorld(nBone);
	lerpToParentsWorld[0] = lerpToParents[0];

	for (int i = 1; i < nBone; ++i)
	{
		XMMATRIX parentToRoot = lerpToParentsWorld[m_SkelParentIndice[i]];
		lerpToParentsWorld[i] = DirectX::XMMatrixMultiply(lerpToParents[i], parentToRoot);
	}
	for (int i = 0; i < nBone; ++i)
	{
		XMMATRIX offset = m_SkelOffset[id][i];
		XMMATRIX toRoot = lerpToParentsWorld[i];

		finalTransforms.push_back(offset * toRoot);
	}

	return finalTransforms;
}
