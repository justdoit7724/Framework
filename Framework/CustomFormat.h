#pragma once
#include "Transform.h"
#include "Geometrics.h"
#include "DX_info.h"

static const D3D11_INPUT_ELEMENT_DESC std_ILayouts[] =
{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMFLOAT3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(XMFLOAT3) * 2, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

struct Vertex {
	XMFLOAT3 pos;
	XMFLOAT3 n;
	XMFLOAT2 tex;

	Vertex() {}
	Vertex(const Vertex& v)
	{
		pos = v.pos;
		n = v.n;
		tex = v.tex;
	}
	Vertex(XMFLOAT3 _pos)
	{
		pos = _pos;
		n = XMFLOAT3(0,0,0);
		tex = XMFLOAT2(0,0);
	}
	Vertex(XMFLOAT3 pos, XMFLOAT3 n, XMFLOAT2 tex) :pos(pos), n(n), tex(tex) {}
};

struct VS_Property
{
	XMMATRIX w;
	XMMATRIX vp;
	XMMATRIX n;
	XMMATRIX tex;

	VS_Property(const XMMATRIX& vp, const XMMATRIX& tex = XMMatrixIdentity())
		:w(XMMatrixIdentity()), vp(vp),n(XMMatrixIdentity()), tex(tex){}
	VS_Property(Transform* transform, const XMMATRIX& vp, const XMMATRIX& tex = XMMatrixIdentity())
		:vp(vp), tex(tex)
	{
		const XMMATRIX s = transform->S();
		const XMMATRIX r = transform->R();
		const XMMATRIX t = transform->T();

		w = s * r * t;
		n = XMMatrixTranspose(XMMatrixInverse(&XMMatrixDeterminant(r), r));
	}
};

#define LIGHT_ENABLED XMFLOAT4(1,1,1,1)
#define LIGHT_DISABLED XMFLOAT4(0,0,0,0)	
#define LIGHT_MAX_EACH 10
struct SHADER_DIRECTIONAL_LIGHT {

	SHADER_DIRECTIONAL_LIGHT() {
		for (int i = 0; i < LIGHT_MAX_EACH; ++i)
		{
			enabled[i] = LIGHT_DISABLED;
		}
	}

	XMFLOAT4 ambient[LIGHT_MAX_EACH];
	XMFLOAT4 diffuse[LIGHT_MAX_EACH];
	XMFLOAT4 specular[LIGHT_MAX_EACH];
	XMFLOAT4 dir[LIGHT_MAX_EACH];
	XMFLOAT4 enabled[LIGHT_MAX_EACH];
};
struct SHADER_POINT_LIGHT {

	SHADER_POINT_LIGHT() {
		for (int i = 0; i < LIGHT_MAX_EACH; ++i)
		{
			enabled[i] = LIGHT_DISABLED;
		}
	}
	XMFLOAT4 ambient[LIGHT_MAX_EACH];
	XMFLOAT4 diffuse[LIGHT_MAX_EACH];
	XMFLOAT4 specular[LIGHT_MAX_EACH];
	XMFLOAT4 pos[LIGHT_MAX_EACH];
	XMFLOAT4 range[LIGHT_MAX_EACH];
	XMFLOAT4 att[LIGHT_MAX_EACH];
	XMFLOAT4 enabled[LIGHT_MAX_EACH];
};
struct SHADER_SPOT_LIGHT {

	SHADER_SPOT_LIGHT() {
		for (int i = 0; i < LIGHT_MAX_EACH; ++i)
		{
			enabled[i] = LIGHT_DISABLED;
		}
	}
	XMFLOAT4 ambient[LIGHT_MAX_EACH];
	XMFLOAT4 diffuse[LIGHT_MAX_EACH];
	XMFLOAT4 specular[LIGHT_MAX_EACH];
	XMFLOAT4 pos[LIGHT_MAX_EACH];
	XMFLOAT4 range[LIGHT_MAX_EACH];
	XMFLOAT4 dir[LIGHT_MAX_EACH];
	XMFLOAT4 spot[LIGHT_MAX_EACH];
	XMFLOAT4 att[LIGHT_MAX_EACH];
	XMFLOAT4 enabled[LIGHT_MAX_EACH];
};

struct ShaderMaterial
{
	XMFLOAT4 diffuse;
	XMFLOAT4 ambient;
	XMFLOAT4 specular;
	XMFLOAT4 reflection;

	ShaderMaterial(XMFLOAT3 d, XMFLOAT3 a, XMFLOAT3 s, float sp, XMFLOAT3 r)
		:diffuse(XMFLOAT4(d.x, d.y, d.z, 0)),ambient(XMFLOAT4(a.x, a.y, a.z, 0)),specular(XMFLOAT4(s.x, s.y, s.z, sp)), reflection(XMFLOAT4(r.x, r.y, r.z, 0)){}
};