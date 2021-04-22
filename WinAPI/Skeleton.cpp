#include "stdafx.h"
#include "Skeleton.h"

void Skeleton::Set(const std::vector<int>& parentIndex, const std::unordered_map<std::string, int>& nameCurIndex, const std::vector<XMMATRIX>& toParent, const std::vector<XMMATRIX>& offset)
{
	m_viParentIndex = parentIndex;
	m_mNameCurIndex = nameCurIndex;
	m_vmatToParent = toParent;
	m_vmatOffset = offset;
}
