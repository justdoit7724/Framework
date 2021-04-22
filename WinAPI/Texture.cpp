#include "stdafx.h"
#include "Texture.h"

void Texture::SetKey(std::string path)
{
	m_path = path;
}

std::string Texture::GetKey()
{
	return m_path;
}
