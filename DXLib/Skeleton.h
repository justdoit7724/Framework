#pragma once
#include "Asset.h"


class Skeleton : public Asset
{
public:
	void Set(
		const std::vector<int>& parentIndex,
		const std::unordered_map<std::string, int>& nameCurIndex,
		const std::vector<XMMATRIX>& toParent,
		const std::vector<XMMATRIX>& offset);

private:
	std::vector<int> m_viParentIndex;
	std::unordered_map<std::string, int> m_mNameCurIndex;
	std::vector<XMMATRIX> m_vmatToParent;
	std::vector<XMMATRIX> m_vmatOffset;
};