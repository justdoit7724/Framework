
#include "pch.h"

#include "Light.h"
#include "ShaderFormat.h"
#include "Camera.h"

#include "Transform.h"
#include "Object.h"
#include "Shader.h"
#include "BlendState.h"
#include "RasterizerState.h"
#include "ShaderReg.h"
#include "DepthStencilState.h"

using namespace DX;

SHADER_DIRECTIONAL_LIGHT DirectionalLight::data;
SHADER_POINT_LIGHT PointLight::data;
SHADER_SPOT_LIGHT SpotLight::data;
ID3D11Buffer* DirectionalLight::cb=nullptr;
ID3D11Buffer* PointLight::cb = nullptr;
ID3D11Buffer* SpotLight::cb = nullptr;

Light::Light()
	:diffuse(XMFLOAT3(1,1,1)), ambient(XMFLOAT3(1,1,1)), specular(XMFLOAT3(0,0,0))
{
}

Light::~Light()
{
}

DirectionalLight::DirectionalLight(XMFLOAT3 a, XMFLOAT3 d, XMFLOAT3 s, XMFLOAT3 dir)
{
	for (int i = 0; i < LIGHT_MAX_EACH; ++i)
	{
		if (data.enabled[i].x == LIGHT_DISABLED)
		{
			id = i;
			break;
		}
	}

	SetAmbient(a);
	SetDiffuse(d);
	SetSpecular(s);
	SetDir(dir);
	Enable(ENABLED);

}

DirectionalLight::~DirectionalLight()
{
	cb->Release();
}

void DirectionalLight::SetAmbient(const XMFLOAT3 & a)
{
	ambient = a;
	data.ambient[id] = XMFLOAT4(a.x, a.y, a.z, 0);
}

void DirectionalLight::SetDiffuse(const XMFLOAT3 & d)
{
	diffuse = d;
	data.diffuse[id] = XMFLOAT4(d.x, d.y, d.z, 0);
}

void DirectionalLight::SetSpecular(const XMFLOAT3 & s)
{
	specular = s;
	data.specular[id] = XMFLOAT4(s.x, s.y, s.z, 0);
}

void DirectionalLight::SetDir(XMFLOAT3 d)
{
	data.dir[id] = XMFLOAT4(d.x, d.y, d.z, 0);
}

void DirectionalLight::Enable(STATE enable)
{
	data.enabled[id] = XMFLOAT4(enable, enable, enable, enable);
}


XMFLOAT3 DirectionalLight::GetDir()const
{
	XMFLOAT4 dir = data.dir[id];

	return XMFLOAT3(dir.x, dir.y, dir.z);
}

void DirectionalLight::Apply()
{
	if (cb == nullptr)
	{
		D3D11_BUFFER_DESC cb_desc;
		cb_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cb_desc.ByteWidth = sizeof(SHADER_DIRECTIONAL_LIGHT);
		cb_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cb_desc.MiscFlags = 0;
		cb_desc.StructureByteStride = 0;
		cb_desc.Usage = D3D11_USAGE_DYNAMIC;
		HRESULT hr = DX_Device->CreateBuffer(&cb_desc, nullptr, &cb);
		r_assert(hr);
	}

	D3D11_MAPPED_SUBRESOURCE mappedData;
	ZeroMemory(&mappedData, sizeof(D3D11_MAPPED_SUBRESOURCE));

	HRESULT hr = DX_DContext->Map(cb, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
	r_assert(hr);
	CopyMemory(mappedData.pData, &data, sizeof(SHADER_DIRECTIONAL_LIGHT));
	DX_DContext->Unmap(cb, 0);

	DX_DContext->PSSetConstantBuffers(SHADER_REG_CB_DIRECTIONAL_LIGHT, 1, &cb);
}



PointLight::PointLight(XMFLOAT3 a, XMFLOAT3 d, XMFLOAT3 s, float range, XMFLOAT3 att, XMFLOAT3 pos)
{
	for (int i = 0; i < LIGHT_MAX_EACH; ++i)
	{
		if (data.info[i].x == LIGHT_DISABLED)
		{
			id = i;
			break;
		}
	}


	SetAmbient(a);
	SetDiffuse(d);
	SetSpecular(s);
	SetPos(pos);
	SetRange(range);
	SetAtt(att);
	Enable(ENABLED);
}

PointLight::~PointLight()
{
	if (cb)
	{
		delete cb;
		cb = nullptr;
	}
}

void PointLight::SetAmbient(const XMFLOAT3 & a)
{
	ambient = a;
	data.ambient[id] = XMFLOAT4(a.x, a.y, a.z, 0);
}

void PointLight::SetDiffuse(const XMFLOAT3 & d)
{
	diffuse = d;
	data.diffuse[id] = XMFLOAT4(d.x, d.y, d.z, 0);
}

void PointLight::SetSpecular(const XMFLOAT3 & s)
{
	specular = s;
	data.specular[id] = XMFLOAT4(s.x, s.y, s.z, 0);
}

void PointLight::SetPos(XMFLOAT3 p)
{
	data.pos[id] = XMFLOAT4(p.x, p.y, p.z, 0);
}

void PointLight::SetRange(float r)
{
	range = r;
	data.info[id].y = r;
}

void PointLight::SetAtt(XMFLOAT3 at)
{
	att = at;
	data.att[id] = XMFLOAT4(at.x, at.y, at.z, 0);
}

void PointLight::Enable(STATE enable)
{
	data.info[id].x = enable;
}

void PointLight::Apply()
{
	HRESULT hr;

	if (cb == nullptr)
	{
		D3D11_BUFFER_DESC cb_desc;
		cb_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cb_desc.ByteWidth = sizeof(SHADER_POINT_LIGHT);
		cb_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cb_desc.MiscFlags = 0;
		cb_desc.StructureByteStride = 0;
		cb_desc.Usage = D3D11_USAGE_DYNAMIC;
		hr = DX_Device->CreateBuffer(&cb_desc, nullptr, &cb);
		r_assert(hr);
	}

	D3D11_MAPPED_SUBRESOURCE mappedData;

	hr = DX_DContext->Map(cb, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
	r_assert(hr);
	CopyMemory(mappedData.pData, &data, sizeof(SHADER_POINT_LIGHT));
	DX_DContext->Unmap(cb, 0);

	DX_DContext->PSSetConstantBuffers(SHADER_REG_CB_POINT_LIGHT, 1, &cb);
}

SpotLight::SpotLight(XMFLOAT3 a, XMFLOAT3 d, XMFLOAT3 s, float r, float spot, float rad, XMFLOAT3 att, XMFLOAT3 pos, XMFLOAT3 dir)
{
	for (int i = 0; i < LIGHT_MAX_EACH; ++i)
	{
		if (data.info[i].x == LIGHT_DISABLED)
		{
			id = i;
			break;
		}
	}

	SetAmbient(a);
	SetDiffuse(d);
	SetSpecular(s);
	SetPos(pos);
	SetDir(dir);
	SetRange(r);
	SetRad(rad);
	SetSpot(spot);
	SetAtt(att);
	Enable(ENABLED);
}

SpotLight::~SpotLight()
{
	if (cb)
	{
		delete cb;
		cb = nullptr;
	}
}

void SpotLight::SetAmbient(const XMFLOAT3 & a)
{
	ambient = a;
	data.ambient[id] = XMFLOAT4(ambient.x, ambient.y, ambient.z, 0);
}

void SpotLight::SetDiffuse(const XMFLOAT3 & d)
{
	diffuse = d;
	data.diffuse[id] = XMFLOAT4(diffuse.x, diffuse.y, diffuse.z, 0);
}

void SpotLight::SetSpecular(const XMFLOAT3 & s)
{
	specular = s;
	data.specular[id] = XMFLOAT4(specular.x, specular.y, specular.z, 0);
}

void SpotLight::SetPos(XMFLOAT3 p)
{
	data.pos[id] = XMFLOAT4(p.x, p.y, p.z, 0);
}

void SpotLight::SetDir(XMFLOAT3 d)
{
	data.dir[id] = XMFLOAT4(d.x, d.y, d.z, 0);
}

void SpotLight::SetRange(float r)
{
	range = r;
	data.info[id].y = r;
}

void SpotLight::SetRad(float r)
{
	rad = r;
	data.info[id].z = r;
}


void SpotLight::SetSpot(float s)
{
	spot = s;
	data.info[id].w = spot;
}

void SpotLight::SetAtt(XMFLOAT3 at)
{
	att = at;
	data.att[id] = XMFLOAT4(att.x, att.y,att.z, 0);
}

void SpotLight::Enable(STATE enable)
{
	data.info[id].x = enable;
}


void SpotLight::Apply()
{
	HRESULT hr;

	if (cb == nullptr)
	{
		D3D11_BUFFER_DESC cb_desc;
		cb_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cb_desc.ByteWidth = sizeof(SHADER_SPOT_LIGHT);
		cb_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cb_desc.MiscFlags = 0;
		cb_desc.StructureByteStride = 0;
		cb_desc.Usage = D3D11_USAGE_DYNAMIC;
		hr = DX_Device->CreateBuffer(&cb_desc, nullptr, &cb);
		r_assert(hr);
	}

	D3D11_MAPPED_SUBRESOURCE mappedData;

	hr = DX_DContext->Map(cb, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
	r_assert(hr);
	CopyMemory(mappedData.pData, &data, sizeof(SHADER_SPOT_LIGHT));
	DX_DContext->Unmap(cb, 0);

	DX_DContext->PSSetConstantBuffers(SHADER_REG_CB_SPOT_LIGHT, 1, &cb);
}

