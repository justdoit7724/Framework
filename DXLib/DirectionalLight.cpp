#include "stdafx.h"
#include "DirectionalLight.h"

DirectionalLight::SHADER_DIRECTIONAL_LIGHT DirectionalLight::m_data;

DirectionalLight::DirectionalLight(Light* main)
	:SubLight(main)
{
	for (int i = 0; i < LIGHT_MAX_EACH; ++i)
	{
		if (m_data.enabled[i].x == LIGHT_DISABLED)
		{
			m_id = i;
			break;
		}
	}

	SetAmbient(XMFLOAT3(0.1, 0.1, 0.1));
	SetDiffuse(XMFLOAT3(0.8, 0.8, 0.8));
	SetSpecular(XMFLOAT3(1.0, 1.0, 1.0));
	SetDir(-DIR_UP);
	SetIntensity(1);
	Enable(true);
}

void DirectionalLight::SetAmbient(XMFLOAT3 a)
{
	m_data.ambient[m_id] = XMFLOAT4(a.x, a.y, a.z, 0);
}

void DirectionalLight::SetDiffuse(XMFLOAT3 d)
{
	m_data.diffuse[m_id] = XMFLOAT4(d.x, d.y, d.z, 0);
}

void DirectionalLight::SetSpecular(XMFLOAT3 s)
{
	m_data.specular[m_id] = XMFLOAT4(s.x, s.y, s.z, 0);
}

void DirectionalLight::SetIntensity(float i)
{
	m_data.intensity[m_id] = XMFLOAT4(i, i, i, i);
}

void DirectionalLight::Enable(bool enable)
{
	if (enable)
		m_data.enabled[m_id] = XMFLOAT4(LIGHT_ENABLED, LIGHT_ENABLED, LIGHT_ENABLED, LIGHT_ENABLED);
	else
		m_data.enabled[m_id] = XMFLOAT4(LIGHT_DISABLED, LIGHT_DISABLED, LIGHT_DISABLED, LIGHT_DISABLED);
}

void DirectionalLight::SetDir(XMFLOAT3 d)
{
	m_data.dir[m_id] = XMFLOAT4(d.x, d.y, d.z, 0);
}

void DirectionalLight::Render(DX::Graphic* graphic)const
{
	std::vector<const void*> datas;
	datas.push_back(&m_data);
	int size = sizeof(SHADER_DIRECTIONAL_LIGHT);
	datas.push_back(&size);
	graphic->SetPipelineData(PIP_REG::PIXEL_LIGHT_DIRECTIONAL, datas);
}
