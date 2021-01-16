#include "stdafx.h"
#include "Scene.h"
#include "Scene.h"
#include "Keyboard.h"


Scene::Scene(DX::Graphic* graphic, const wchar_t* key)
	:m_dxGraphic(graphic), m_key(key),m_iWidth(0), m_iHeight(0), m_iMouseWheel(0)
{
}

Scene::~Scene()
{
}
