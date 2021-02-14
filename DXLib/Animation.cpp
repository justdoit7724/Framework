#include "pch.h"
#include "Animation.h"

using namespace DX;

int SkinnedData::BoneCount() const
{
	return 0;
}

float SkinnedData::GetClipStartTime(const std::string clipName) const
{
	return 0.0f;
}

float SkinnedData::GetClipEndTime(const std::string clipName) const
{
	return 0.0f;
}

void SkinnedData::Set(std::vector<int>& boneHierarchy, std::vector<XMFLOAT4X4>& boneOffsets, std::map<std::string, AnimationClip>& animations)
{
}

void SkinnedData::GetFinalTransforms(const std::string& clipName, float timePos, std::vector<XMMATRIX>& finalTransforms) const
{
	finalTransforms.clear();

	int nBone = m_BoneOffsets.size();

	std::vector<XMMATRIX> toParentTransforms(nBone);

	const AnimationClip& clip = m_Animations.find(clipName)->second;
	clip.Interpolate(timePos, &toParentTransforms);

	std::vector<XMMATRIX> toRootTransforms(nBone);
	toRootTransforms[0] = toParentTransforms[0];

	for (int i = 1; i < nBone; ++i)
	{
		XMMATRIX toParent = toParentTransforms[i];
		XMMATRIX parentToRoot = toRootTransforms[m_BoneHierarchy[i]];
		toRootTransforms[i] = XMMatrixMultiply(toParent, parentToRoot);
	}
	for (int i = 0; i < nBone; ++i)
	{
		XMMATRIX offset = XMLoadFloat4x4(&m_BoneOffsets[i]);
		XMMATRIX toRoot = toRootTransforms[i];
		finalTransforms.push_back(toRoot);
	}
}
