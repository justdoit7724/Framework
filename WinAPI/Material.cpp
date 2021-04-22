#include "stdafx.h"
#include "Material.h"

void Material::SetDiff(std::string diff)
{
	m_diff = diff;
}

void Material::SetNorm(std::string norm)
{
	m_norm = norm;
}

std::string Material::GetDiff()
{
	return m_diff;
}

std::string Material::GetNorm()
{
	return m_norm;
}
