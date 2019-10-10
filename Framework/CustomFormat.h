#pragma once
#include "Geometrics.h"
#include "DX_info.h"

static const D3D11_INPUT_ELEMENT_DESC simple_ILayouts[] =
{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};
static const D3D11_INPUT_ELEMENT_DESC Std_ILayouts[] =
{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMFLOAT3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(XMFLOAT3) * 2, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,sizeof(XMFLOAT3)*2+sizeof(XMFLOAT2), D3D11_INPUT_PER_VERTEX_DATA,0}
};

struct Vertex {
	XMFLOAT3 pos;
	XMFLOAT3 n;
	XMFLOAT2 tex;
	XMFLOAT3 tangent;

	Vertex() {}
	Vertex(const Vertex& v)
	{
		pos = v.pos;
		n = v.n;
		tex = v.tex;
		tangent = v.tangent;
	}
	/*Vertex(XMFLOAT3 _pos)
	{
		pos = _pos;
		n = XMFLOAT3(0,0,0);
		tex = XMFLOAT2(0,0);
		tangent = XMFLOAT3(0, 0, 0);
	}*/
	Vertex(XMFLOAT3 pos, XMFLOAT3 n, XMFLOAT2 tex, XMFLOAT3 tangent) :pos(pos), n(n), tex(tex), tangent(tangent) {}
};

struct SHADER_STD_TRANSF
{
	XMMATRIX w;
	XMMATRIX vp;
	XMMATRIX n;
	XMMATRIX tex;

	SHADER_STD_TRANSF(const XMMATRIX& vp, const XMMATRIX& tex)
		:w(XMMatrixIdentity()), vp(vp),n(XMMatrixIdentity()), tex(tex){}
	SHADER_STD_TRANSF(const XMMATRIX& world, const XMMATRIX& vp, const XMMATRIX& tex)
		:w(world), vp(vp), tex(tex)
	{
		n = XMMatrixTranspose(XMMatrixInverse(&XMMatrixDeterminant(world), world));
	}
};
struct SHADER_PT_TRANSF
{
	XMMATRIX w;
	XMMATRIX vp;
	XMMATRIX n;
	XMMATRIX ui;
	XMMATRIX pt_vp;

	SHADER_PT_TRANSF(const XMMATRIX& world, const XMMATRIX& vp, const XMMATRIX& pt_vp, const XMMATRIX& ui)
		:w(world), vp(vp), pt_vp(pt_vp), ui(ui)
	{
		n = XMMatrixTranspose(XMMatrixInverse(&XMMatrixDeterminant(world), world));
	}
};
struct VS_Simple_Property
{
	XMMATRIX worldMat;
	XMMATRIX vpMat;

	VS_Simple_Property(const XMMATRIX& w, const XMMATRIX& vp)
		: worldMat(w), vpMat(vp){}
};

#define LIGHT_ENABLED DirectX::XMFLOAT4(1,1,1,1)
#define LIGHT_DISABLED DirectX::XMFLOAT4(0,0,0,0)	
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

struct SHADER_MATERIAL
{
private:
	XMFLOAT4 diffuse;
	XMFLOAT4 ambient;
	XMFLOAT4 specular;
	XMFLOAT4 reflection;

public:
	SHADER_MATERIAL(XMFLOAT3 d, float transparency, XMFLOAT3 a, XMFLOAT3 s, float sp, XMFLOAT3 r)
		:diffuse(XMFLOAT4(d.x, d.y, d.z, transparency)),ambient(XMFLOAT4(a.x, a.y, a.z, 0)),specular(XMFLOAT4(s.x, s.y, s.z, sp)), reflection(XMFLOAT4(r.x, r.y, r.z, 0)){}
	void SetTransparency(float t)
	{
		diffuse.w = t;
	}
	void SetSpecPower(float p)
	{
		specular.w = p;
	}
	void SetDiffuse(XMFLOAT3 d)
	{
		diffuse.x = d.x;
		diffuse.y = d.y;
		diffuse.z = d.z;
	}
	void SetSpec(XMFLOAT3 s)
	{
		specular.x = s.x;
		specular.y = s.y;
		specular.z = s.z;
	}
	void SetAmbient(XMFLOAT3 a)
	{
		ambient.x = a.x;
		ambient.y = a.y;
		ambient.z = a.z;
	}
};