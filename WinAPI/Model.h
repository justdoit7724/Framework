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
	XMMATRIX Interpolate(float t)const
	{
		if (keyFrames.empty())
		{
			return XMMatrixIdentity();
		}

		XMVECTOR s;
		XMVECTOR p;
		XMVECTOR q;

		if (t <= keyFrames.front().timePos)
		{
			s = XMLoadFloat3(&keyFrames.front().scale);
			p = XMLoadFloat3(&keyFrames.front().translation);
			q = XMLoadFloat4(&keyFrames.front().rotationQuat);
		}
		else if (t > keyFrames.back().timePos)
		{
			s = XMLoadFloat3(&keyFrames.back().scale);
			p = XMLoadFloat3(&keyFrames.back().translation);
			q = XMLoadFloat4(&keyFrames.back().rotationQuat);
		}
		else
		{
			for (int i = 0; i < keyFrames.size() - 1; i++)
			{
				if (keyFrames[i].timePos <= t && t <= keyFrames[i + 1].timePos)
				{
					float lerpT = (t - keyFrames[i].timePos) / (keyFrames[i + 1].timePos - keyFrames[i].timePos);

					s = XMVectorLerp(
						XMLoadFloat3(&keyFrames[i].scale),
						XMLoadFloat3(&keyFrames[i + 1].scale),
						lerpT);
					p = XMVectorLerp(
						XMLoadFloat3(&keyFrames[i].translation),
						XMLoadFloat3(&keyFrames[i + 1].translation),
						lerpT);
					q = XMQuaternionSlerp(
						XMLoadFloat4(&keyFrames[i].rotationQuat),
						XMLoadFloat4(&keyFrames[i + 1].rotationQuat),
						lerpT);

					break;
				}
			}
		}

		XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		return XMMatrixAffineTransformation(s, zero, q, p);
	}

	std::vector<KeyFrame> keyFrames;
};
struct AnimationClip
{
	void SetAnimation(std::vector<BoneAnimation>&& anims) {
		boneAnims = anims;
	}
	void Interpolate(float t, std::vector<XMMATRIX>& boneTransforms)const {
		boneTransforms.clear();

		for (int i = 0; i < boneAnims.size(); ++i)
		{
			XMMATRIX boneMat = boneAnims[i].Interpolate(t);
			boneTransforms.push_back(boneMat);
		}
	}


	std::vector<BoneAnimation> boneAnims;
};

class Model :
    public Asset
{
public:
	~Model();

	//mesh
public:
	void AddSubMesh(int id, const std::vector<DX::Vertex>& vertice, const std::vector<UINT>& indice);
private:
	struct SubMesh
	{
		std::vector<DX::Vertex> vertice;
		std::vector<UINT> indice;
	};
	std::vector<SubMesh> m_subMeshes;
  

	//skeleton
public:
	void SetSkeleton(
		const std::vector<int>&& parentIndice,
		const std::vector<std::string>&& names,
		const std::vector<XMMATRIX>&& toParents,
		const std::vector<XMMATRIX>&& offsets);
private:
	std::vector<int> m_SkelParentIndice;
	std::vector<std::string> m_SkelNames;
	std::vector<XMMATRIX> m_SkelToParents;
	std::vector<XMMATRIX> m_SkelOffset;

	//animations
public:
	void SetAnimations(std::string name, AnimationClip clip);
	std::vector<XMMATRIX> GetFinalTransform(std::string name, float t);
private:
	std::unordered_map<std::string, AnimationClip> m_animClips;
};

