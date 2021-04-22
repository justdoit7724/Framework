#include "stdafx.h"
#include "SpotLight.h"

SpotLight::SHADER_SPOT_LIGHT SpotLight::m_data;

SpotLight::SpotLight(Light* main)
	:SubLight(main)
{
	for (int i = 0; i < LIGHT_MAX_EACH; ++i)
	{
		if (m_data.info[i].x == LIGHT_DISABLED)
		{
			m_id = i;
			break;
		}
	}

	SetAmbient(XMFLOAT3(0.2, 0.2, 0.2));
	SetDiffuse(XMFLOAT3(0.8, 0.8, 0.8));
	SetSpecular(XMFLOAT3(1.0, 1.0, 1.0));
	SetPos(XMFLOAT3(0, 0, 0));
	SetDir(-DIR_UP);
	SetRange(0);
	SetRad(100);
	SetSpot(XM_PIDIV2);
	SetAtt(XMFLOAT3(0.1, 0.1, 0.1));
	SetIntensity(1);
	Enable(true);
}

void SpotLight::SetAmbient(XMFLOAT3 a)
{
	m_data.ambient[m_id] = XMFLOAT4(a.x, a.y, a.z, 0);
}

void SpotLight::SetDiffuse(XMFLOAT3 d)
{
	m_data.diffuse[m_id] = XMFLOAT4(d.x, d.y, d.z, 0);
}

void SpotLight::SetSpecular(XMFLOAT3 s)
{
	m_data.specular[m_id] = XMFLOAT4(s.x, s.y, s.z, 0);
}

void SpotLight::SetIntensity(float i)
{
	m_data.intensity[m_id] = XMFLOAT4(i, i, i, i);
}

void SpotLight::SetPos(XMFLOAT3 p)
{
	m_data.pos[m_id] = XMFLOAT4(p.x, p.y, p.z, 0);
}

void SpotLight::SetDir(XMFLOAT3 d)
{
	m_data.dir[m_id] = XMFLOAT4(d.x, d.y, d.z, 0);
}

void SpotLight::SetRange(float r)
{
	m_data.info[m_id].y = r;
}

void SpotLight::SetRad(float r)
{
	m_data.info[m_id].z = r;
}


void SpotLight::SetSpot(float s)
{
	m_data.info[m_id].w = s;
}

void SpotLight::SetAtt(XMFLOAT3 at)
{
	m_data.att[m_id] = XMFLOAT4(at.x, at.y, at.z, 0);
}

void SpotLight::Enable(bool enable)
{
	if (enable)
		m_data.info[m_id].x = LIGHT_ENABLED;
	else
		m_data.info[m_id].x = LIGHT_DISABLED;
}

void SpotLight::Render(DX::Graphic* graphic)const
{
	std::vector<const void*> datas;
	datas.push_back(&m_data);
	int size = sizeof(SHADER_SPOT_LIGHT);
	datas.push_back(&size);
	graphic->SetPipelineData(PIP_REG::PIXEL_LIGHT_SPOT, datas);
}
