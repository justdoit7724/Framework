#include "stdafx.h"
#include "Scene.h"
#include "Scene.h"
#include "Keyboard.h"

void Scene::ReleaseKey(WPARAM wparam)
{
	m_keyboard->Release(wparam);
}

void Scene::PressKey(WPARAM wparam)
{
	m_keyboard->Press(wparam);
}

void Scene::Mouse_LBtnDown()
{
	m_bPressingLMouse = true;
}

void Scene::Mouse_LBtnUp()
{
	m_bPressingLMouse = false;
}

void Scene::Mouse_RBtnDown()
{
	m_bPressingRMouse = true;
}

void Scene::Mouse_RBtnUp()
{
	m_bPressingRMouse = false;
}

void Scene::Mouse_UpdatePt(LPARAM lparam)
{
	POINTS p = MAKEPOINTS(lparam);
	m_scnMousePos.x = p.x;
	m_scnMousePos.y = p.y;
}

void Scene::Mouse_Wheel(WPARAM wparam)
{
}

Scene::Scene(ID3D11Device* device, ID3D11DeviceContext* dContext, const wchar_t* key)
	:m_device(device), m_dContext(dContext), m_key(key)
{
}
