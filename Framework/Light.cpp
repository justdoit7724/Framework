#include "Light.h"
#include "ShaderFormat.h"
#include "Transform.h"

SHADER_DIRECTIONAL_LIGHT DirectionalLight::data;
SHADER_POINT_LIGHT PointLight::data;
SHADER_SPOT_LIGHT SpotLight::data;
ID3D11Buffer* DirectionalLight::cb;
ID3D11Buffer* PointLight::cb;
ID3D11Buffer* SpotLight::cb;


Light::Light(XMFLOAT3 a, XMFLOAT3 d, XMFLOAT3 s)
	:ambient(a), diffuse(d), specular(s)
{


	transform = new Transform();
}

Light::~Light()
{
	delete transform;
}

DirectionalLight::DirectionalLight(XMFLOAT3 a, XMFLOAT3 d, XMFLOAT3 s, XMFLOAT3 dir)
	:Light(a,d,s)
{
	D3D11_BUFFER_DESC cb_desc;
	cb_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb_desc.ByteWidth = sizeof(SHADER_DIRECTIONAL_LIGHT);
	cb_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb_desc.MiscFlags = 0;
	cb_desc.StructureByteStride = 0;
	cb_desc.Usage = D3D11_USAGE_DYNAMIC;
	r_assert(
		DX_Device->CreateBuffer(&cb_desc, nullptr, &cb)
	);

	for (int i = 0; i < LIGHT_MAX_EACH; ++i)
	{
		if (data.enabled[i].x == LIGHT_DISABLED.x)
		{
			id = i;
			break;
		}
	}
	transform->SetRot(dir);

	data.ambient[id] = XMFLOAT4(a.x,a.y,a.z, 0);
	data.diffuse[id] = XMFLOAT4(d.x,d.y,d.z, 0);
	data.specular[id] = XMFLOAT4(s.x,s.y,s.z, 0);
	XMFLOAT3 forward = transform->GetForward();
	data.dir[id] = XMFLOAT4(forward.x, forward.y, forward.z, 0);
	data.enabled[id] = LIGHT_ENABLED;
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
	XMFLOAT3 up = UP;
	if (d.x == 0 && d.y == 0)
	{
		up = transform->GetUp();
	}
	XMFLOAT3 right = Cross(up,d);


	transform->SetRot(d, Cross(d,right), right);
	data.dir[id] = XMFLOAT4(d.x, d.y, d.z, 0);
}

void DirectionalLight::Apply()
{
	D3D11_MAPPED_SUBRESOURCE mappedData;

	r_assert(DX_DContext->Map(cb, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	CopyMemory(mappedData.pData, &data, sizeof(SHADER_DIRECTIONAL_LIGHT));
	DX_DContext->Unmap(cb, 0);

	DX_DContext->PSSetConstantBuffers(SHADER_REG_PS_DIRECTIONAL_LIGHT, 1, &cb);
}



PointLight::PointLight(XMFLOAT3 a, XMFLOAT3 d, XMFLOAT3 s, XMFLOAT3 pos, float range, XMFLOAT3 att)
	:Light(a,d,s),range(range),att(att)
{
	for (int i = 0; i < LIGHT_MAX_EACH; ++i)
	{
		if (data.enabled[i].x == LIGHT_DISABLED.x)
		{
			id = i;
			break;
		}
	}

	transform->SetTranslation(pos);

	data.ambient[id] = XMFLOAT4(a.x, a.y, a.z, 0);
	data.diffuse[id] = XMFLOAT4(d.x, d.y, d.z, 0);
	data.specular[id] = XMFLOAT4(s.x, s.y, s.z, 0);
	data.pos[id] = XMFLOAT4(pos.x, pos.y, pos.z, 0);
	data.range[id] = XMFLOAT4(range, range, range, 0);
	data.att[id] = XMFLOAT4(att.x, att.y, att.z, 0);
	data.enabled[id] = LIGHT_ENABLED;
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
	transform->SetTranslation(p);
	XMFLOAT3 pos = transform->GetPos();
	data.pos[id] = XMFLOAT4(pos.x, pos.y, pos.z, 0);
}

void PointLight::SetRange(float r)
{
	range = r;
	data.range[id] = XMFLOAT4(range, range, range, 0);
}

void PointLight::SetAtt(XMFLOAT3 at)
{
	att = at;
	data.att[id] = XMFLOAT4(at.x, at.y, at.z, 0);
}

void PointLight::Apply()
{
	DX_DContext->PSSetConstantBuffers(SHADER_REG_PS_POINT_LIGHT, 1, &cb);
}

SpotLight::SpotLight(XMFLOAT3 a, XMFLOAT3 d, XMFLOAT3 s, XMFLOAT3 pos, XMFLOAT3 dir, float r, float spot, XMFLOAT3 att)
	:Light(a,d,s), range(r), spot(spot), att(att)
{

	for (int i = 0; i < LIGHT_MAX_EACH; ++i)
	{
		if (data.enabled[i].x == LIGHT_DISABLED.x)
		{
			id = i;
			break;
		}
	}

	transform->SetTranslation(pos);
	transform->SetRot(dir);

	data.ambient[id] = XMFLOAT4(ambient.x, ambient.y, ambient.z, 0);
	data.diffuse[id] = XMFLOAT4(diffuse.x, diffuse.y, diffuse.z, 0);
	data.specular[id] = XMFLOAT4(specular.x, specular.y, specular.z, 0);
	data.pos[id] = XMFLOAT4(pos.x, pos.y, pos.z, 0);
	XMFLOAT3 forward = transform->GetForward();
	data.dir[id] = XMFLOAT4(forward.x, forward.y, forward.z, 0);
	data.spot[id] = XMFLOAT4(spot, spot, spot, 0);
	data.range[id] = XMFLOAT4(range, range, range, 0);
	data.att[id] = XMFLOAT4(att.x, att.y, att.z, 0);
	data.enabled[id] = LIGHT_ENABLED;
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
	transform->SetTranslation(p);
	data.pos[id] = XMFLOAT4(p.x, p.y, p.z, 0);
}

void SpotLight::SetDir(XMFLOAT3 d)
{
	transform->SetRot(d);
	data.dir[id] = XMFLOAT4(d.x, d.y, d.z, 0);
}

void SpotLight::SetRange(float r)
{
	range = r;
	data.range[id] = XMFLOAT4(range, range, range, 0);
}

void SpotLight::SetSpot(float s)
{
	spot = s;
	data.spot[id] = XMFLOAT4(spot, spot, spot, 0);
}

void SpotLight::SetAtt(XMFLOAT3 at)
{
	att = at;
	data.att[id] = XMFLOAT4(att.x, att.y,att.z, 0);
}

void SpotLight::Apply()
{
	DX_DContext->PSSetConstantBuffers(SHADER_REG_PS_SPOT_LIGHT, 1, &cb);
}
