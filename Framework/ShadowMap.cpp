#include "ShadowMap.h"

ShadowMap::ShadowMap(UINT width, UINT height)
	:width(width), height(height), depthSRV(nullptr), depthDSV(nullptr)
{
}

ShadowMap::~ShadowMap()
{
}

ID3D11ShaderResourceView* ShadowMap::Depth()
{
	return nullptr;
}

void ShadowMap::BindDSVAndSetNullRT()
{
}
//
//ShadowMap& ShadowMap::operator=(const ShadowMap& rhs)
//{
//	// TODO: insert return statement here
//}
