#include "stdafx.h"
#include "Animation.h"

Animation::Animation(const std::vector<BoneAnimation>& anim)
{
}

/*
void Animation::Set(std::string key, AnimationClip clip)
{
	if (m_mClip.find(key) == m_mClip.end())
	{
		m_mClip.insert(std::pair<std::string, AnimationClip>(key, clip));
	}
	else
	{
		m_mClip[key] = clip;
	}
}

int Animation::BoneCount() const
{
	return 0;
}

float Animation::GetClipStartTime(const std::string clipName) const
{
	if (m_mClip.find(clipName) == m_mClip.end())
		return -1;

	return m_mClip.at(clipName).GetClipStartTime();
}

float Animation::GetClipEndTime(const std::string clipName) const
{
	if (m_mClip.find(clipName) == m_mClip.end())
		return -1;

	return m_mClip.at(clipName).GetClipEndTime();
}

void Animation::GetFinalTransforms(const std::string& clipName, float timePos, std::vector<XMMATRIX>& finalTransforms) const
{
	finalTransforms.clear();

	int nBone = m_BoneOffsets.size();

	std::vector<XMMATRIX> toParentTransforms(nBone);

	const AnimationClip& clip = m_mClip.find(clipName)->second;
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
		XMMATRIX offset = m_BoneOffsets[i];
		XMMATRIX toRoot = toRootTransforms[i];

		finalTransforms.push_back(offset * toRoot);
	}
}
*/
