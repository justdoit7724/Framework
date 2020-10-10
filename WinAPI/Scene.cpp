#include "stdafx.h"
#include "Scene.h"
#include "Scene.h"
#include "Keyboard.h"


Scene::Scene(ID3D11Device* device, ID3D11DeviceContext* dContext, const wchar_t* key)
	:m_device(device), m_dContext(dContext), m_key(key)
{
}
