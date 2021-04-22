#include "stdafx.h"
#include "MeshRenderer.h"


MeshRenderer::MeshRenderer(Object* owner)
	: Component(CMPNT_KIND::MeshRenderer, owner)
{
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::Update(DX::Graphic* graphic)
{
}

void MeshRenderer::Render(DX::Graphic* graphic)const
{
}

void MeshRenderer::SetMaterial(UINT id, std::string key)
{
	m_matKeys.reserve(id + 1);

	m_matKeys[id] = key;
}

std::string MeshRenderer::GetMaterial(UINT id)
{
	return m_matKeys[id];
}
