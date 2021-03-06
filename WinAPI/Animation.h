#pragma once
#include "Asset.h"

struct KeyFrame
{
	float timePos;
	XMFLOAT3 translation;
	XMFLOAT3 scale;
	XMFLOAT4 rotationQuat;
};
struct BoneAnimation
{
	float GetStartTime() const
	{
		return Keyframes.front().timePos;
	}
	float GetEndTime() const
	{
		return Keyframes.back().timePos;
	}
	void Interpolate(float t, XMMATRIX& M)const
	{
		if (Keyframes.empty())
		{
			M = XMMatrixIdentity();
			return;
		}

		XMVECTOR s;
		XMVECTOR p;
		XMVECTOR q;

		if (t <= Keyframes.front().timePos)
		{
			s = XMLoadFloat3(&Keyframes.front().scale);
			p = XMLoadFloat3(&Keyframes.front().translation);
			q = XMLoadFloat4(&Keyframes.front().rotationQuat);
		}
		else if (t > Keyframes.back().timePos)
		{
			s = XMLoadFloat3(&Keyframes.back().scale);
			p = XMLoadFloat3(&Keyframes.back().translation);
			q = XMLoadFloat4(&Keyframes.back().rotationQuat);
		}
		else // main
		{
			for (int i = 0; i < Keyframes.size() - 1; i++)
			{
				if (Keyframes[i].timePos <= t && t <= Keyframes[i + 1].timePos)
				{
					float lerpT = (t - Keyframes[i].timePos) / (Keyframes[i + 1].timePos - Keyframes[i].timePos);

					s = XMVectorLerp(
						XMLoadFloat3(&Keyframes[i].scale),
						XMLoadFloat3(&Keyframes[i + 1].scale),
						lerpT);
					p = XMVectorLerp(
						XMLoadFloat3(&Keyframes[i].translation),
						XMLoadFloat3(&Keyframes[i + 1].translation),
						lerpT);
					q = XMQuaternionSlerp(
						XMLoadFloat4(&Keyframes[i].rotationQuat),
						XMLoadFloat4(&Keyframes[i + 1].rotationQuat),
						lerpT);

					break;
				}
			}
		}

		XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		M = XMMatrixAffineTransformation(s, zero, q, p);
	}
	std::vector<KeyFrame> Keyframes;
};

class Animation : public Asset 
{
public:
	Animation(const std::vector<BoneAnimation>& anim);

	float GetClipStartTime()const
	{
		float minTime = FLT_MAX;
		for (int i = 0; i < BoneAnimations.size(); ++i)
		{
			float curTime = BoneAnimations[i].GetStartTime();
			if (curTime < minTime)
				minTime = curTime;
		}
		return minTime;
	}
	float GetClipEndTime()const
	{
		float maxTime = FLT_MIN;
		for (int i = 0; i < BoneAnimations.size(); ++i)
		{
			float curTime = BoneAnimations[i].GetEndTime();
			if (curTime > maxTime)
				maxTime = curTime;
		}
		return maxTime;
	}
	void Interpolate(float t, std::vector<XMMATRIX>* boneTransforms)const
	{
		boneTransforms->clear();

		for (int i = 0; i < BoneAnimations.size(); ++i)
		{
			XMMATRIX boneMat;
			BoneAnimations[i].Interpolate(t, boneMat);
			boneTransforms->push_back(boneMat);
		}
	}

private:
	std::vector<BoneAnimation> BoneAnimations;
};

/*
class Animation
{
public:
	Animation();
	~Animation();

	void Set(std::string key, AnimationClip clip);

	int BoneCount()const;

	float GetClipStartTime(const std::string clipName)const;
	float GetClipEndTime(const std::string clipName)const;

	void GetFinalTransforms(const std::string& clipName, float timePos, std::vector<XMMATRIX>& finalTransforms)const;

private:
	std::vector<int> m_BoneHierarchy;
	std::vector<XMMATRIX> m_BoneOffsets;
	std::unordered_map<std::string, AnimationClip> m_mClip;
};
*/