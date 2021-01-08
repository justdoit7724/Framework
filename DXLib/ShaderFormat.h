#pragma once


#define LIGHT_ENABLED 1
#define LIGHT_DISABLED 0
#define LIGHT_MAX_EACH 3

namespace DX {
	static const D3D11_INPUT_ELEMENT_DESC simple_ILayouts[1] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	static const D3D11_INPUT_ELEMENT_DESC UI_ILayouts[2] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(XMFLOAT3), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	static const D3D11_INPUT_ELEMENT_DESC Std_ILayouts[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(XMFLOAT3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMFLOAT3) + sizeof(XMFLOAT2), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,sizeof(XMFLOAT3) * 2 + sizeof(XMFLOAT2), D3D11_INPUT_PER_VERTEX_DATA,0}
	};

	struct Vertex {
		XMFLOAT3 pos;
		XMFLOAT2 tex;
		XMFLOAT3 n;
		//XMFLOAT3 tangent;

		Vertex() :pos(XMFLOAT3(0, 0, 0)), n(XMFLOAT3(0, 0, 0)), tex(XMFLOAT2(0, 0)) {}
		Vertex(const Vertex& v)
		{
			pos = v.pos;
			n = v.n;
			tex = v.tex;
		}
		Vertex(XMFLOAT3 _pos)
		{
			pos = _pos;
			n = XMFLOAT3(0, 0, 0);
			tex = XMFLOAT2(0, 0);
		}
		Vertex(XMFLOAT3 pos, XMFLOAT3 n, XMFLOAT2 tex, XMFLOAT3 tangent) :pos(pos), n(n), tex(tex) {}
	};

	struct SHADER_STD_TRANSF
	{
		XMMATRIX w;
		XMMATRIX v;
		XMMATRIX p;
		XMMATRIX n;
		float fNear;
		float fFar;
		float fAspect;
		float fRatio;

		SHADER_STD_TRANSF(XMMATRIX v, XMMATRIX p, float fNear, float fFar, float fAspect, float fRatio)
			:v(v), p(p), fNear(fNear), fFar(fFar), fAspect(fAspect), fRatio(fRatio)
		{
			const XMMATRIX& mId = DirectX::XMMatrixIdentity();
			w = mId;
			n = mId;
		}
		SHADER_STD_TRANSF(XMMATRIX world, XMMATRIX v, XMMATRIX p, float fNear, float fFar, float fAspect, float fRatio)
			:w(world), v(v), p(p), fNear(fNear), fFar(fFar), fAspect(fAspect), fRatio(fRatio)
		{
			XMVECTOR det = DirectX::XMMatrixDeterminant(world);
			n = DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(&det, world));
		}
	};
	struct SHADER_PT_TRANSF
	{
		XMMATRIX w;
		XMMATRIX vp;
		XMMATRIX n;
		XMMATRIX ui;
		XMMATRIX dLightVPT;
		XMMATRIX pLightP;

		SHADER_PT_TRANSF(const XMMATRIX& world, const XMMATRIX& vp, const XMMATRIX& ui, const XMMATRIX& dLightVP, const XMMATRIX& pLightP)
			:w(world),
			vp(vp),
			ui(ui),
			dLightVPT(dLightVP* XMMATRIX(
				0.5f, 0, 0, 0,
				0, -0.5f, 0, 0,
				0, 0, 1, 0,
				0.5f, 0.5f, 0, 1)),
			pLightP(pLightP)
		{
			n = DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(&DirectX::XMMatrixDeterminant(world), world));
		}
	};
	struct VS_Simple_Property
	{
		XMMATRIX worldMat;
		XMMATRIX vpMat;

		VS_Simple_Property(const XMMATRIX& w, const XMMATRIX& vp)
			: worldMat(w), vpMat(vp) {}
	};

	struct SHADER_DIRECTIONAL_LIGHT {

		SHADER_DIRECTIONAL_LIGHT() {


			ZeroMemory(ambient, sizeof(XMFLOAT4) * LIGHT_MAX_EACH);
			ZeroMemory(diffuse, sizeof(XMFLOAT4) * LIGHT_MAX_EACH);
			ZeroMemory(specular, sizeof(XMFLOAT4) * LIGHT_MAX_EACH);
			ZeroMemory(dir, sizeof(XMFLOAT4) * LIGHT_MAX_EACH);
			ZeroMemory(enabled, sizeof(XMFLOAT4) * LIGHT_MAX_EACH);
			ZeroMemory(intensity, sizeof(XMFLOAT4) * LIGHT_MAX_EACH);

			for (int i = 0; i < LIGHT_MAX_EACH; ++i)
			{
				enabled[i] = XMFLOAT4(LIGHT_DISABLED, 0, 0, 0);
			}
		}

		XMFLOAT4 ambient[LIGHT_MAX_EACH];
		XMFLOAT4 diffuse[LIGHT_MAX_EACH];
		XMFLOAT4 specular[LIGHT_MAX_EACH];
		XMFLOAT4 dir[LIGHT_MAX_EACH];
		XMFLOAT4 enabled[LIGHT_MAX_EACH]; // enable
		XMFLOAT4 intensity[LIGHT_MAX_EACH];

	};
	struct SHADER_POINT_LIGHT {

		SHADER_POINT_LIGHT()
		{

			ZeroMemory(ambient, sizeof(XMFLOAT4) * LIGHT_MAX_EACH);
			ZeroMemory(diffuse, sizeof(XMFLOAT4) * LIGHT_MAX_EACH);
			ZeroMemory(specular, sizeof(XMFLOAT4) * LIGHT_MAX_EACH);
			ZeroMemory(pos, sizeof(XMFLOAT4) * LIGHT_MAX_EACH);
			ZeroMemory(info, sizeof(XMFLOAT4) * LIGHT_MAX_EACH);
			ZeroMemory(att, sizeof(XMFLOAT4) * LIGHT_MAX_EACH);
			ZeroMemory(intensity, sizeof(XMFLOAT4) * LIGHT_MAX_EACH);

			for (int i = 0; i < LIGHT_MAX_EACH; ++i)
			{
				info[i].x = LIGHT_DISABLED;
			}
		}

		XMFLOAT4 ambient[LIGHT_MAX_EACH];
		XMFLOAT4 diffuse[LIGHT_MAX_EACH];
		XMFLOAT4 specular[LIGHT_MAX_EACH];
		XMFLOAT4 pos[LIGHT_MAX_EACH];
		XMFLOAT4 info[LIGHT_MAX_EACH]; // enable
		XMFLOAT4 att[LIGHT_MAX_EACH];
		XMFLOAT4 intensity[LIGHT_MAX_EACH];

	};
	struct SHADER_SPOT_LIGHT {

		SHADER_SPOT_LIGHT()
		{
			ZeroMemory(ambient, sizeof(XMFLOAT4) * LIGHT_MAX_EACH);
			ZeroMemory(diffuse, sizeof(XMFLOAT4) * LIGHT_MAX_EACH);
			ZeroMemory(specular, sizeof(XMFLOAT4) * LIGHT_MAX_EACH);
			ZeroMemory(pos, sizeof(XMFLOAT4) * LIGHT_MAX_EACH);
			ZeroMemory(info, sizeof(XMFLOAT4) * LIGHT_MAX_EACH);
			ZeroMemory(dir, sizeof(XMFLOAT4) * LIGHT_MAX_EACH);
			ZeroMemory(att, sizeof(XMFLOAT4) * LIGHT_MAX_EACH);
			ZeroMemory(intensity, sizeof(XMFLOAT4) * LIGHT_MAX_EACH);

			for (int i = 0; i < LIGHT_MAX_EACH; ++i)
			{
				info[i].x = LIGHT_DISABLED;
			}
		}

		XMFLOAT4 ambient[LIGHT_MAX_EACH];
		XMFLOAT4 diffuse[LIGHT_MAX_EACH];
		XMFLOAT4 specular[LIGHT_MAX_EACH];
		XMFLOAT4 pos[LIGHT_MAX_EACH];
		XMFLOAT4 info[LIGHT_MAX_EACH]; // enabled,range,rad,spot
		XMFLOAT4 dir[LIGHT_MAX_EACH];
		XMFLOAT4 att[LIGHT_MAX_EACH];
		XMFLOAT4 intensity[LIGHT_MAX_EACH];
	};

	struct SHADER_MATERIAL
	{
	private:
		XMFLOAT4 diffuse;
		XMFLOAT4 ambient;
		XMFLOAT4 specular;

	public:
		SHADER_MATERIAL() {}
		SHADER_MATERIAL(XMFLOAT3 d, float transparency, XMFLOAT3 a, XMFLOAT3 s)
			:
			diffuse(XMFLOAT4(d.x, d.y, d.z, transparency)),
			ambient(XMFLOAT4(a.x, a.y, a.z, 0)),
			specular(XMFLOAT4(s.x, s.y, s.z, 0))
		{}
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
}