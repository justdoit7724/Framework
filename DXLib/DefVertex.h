#pragma once

#include <DirectXMath.h>

namespace DX
{
	struct Vertex {
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 n;
		DirectX::XMFLOAT2 tex;
		unsigned int boneId;
		//XMFLOAT3 tangent;

		Vertex() :pos(DirectX::XMFLOAT3(0, 0, 0)), n(DirectX::XMFLOAT3(0, 0, 0)), tex(DirectX::XMFLOAT2(0, 0)), boneId(0) {}
		Vertex(const Vertex& v)
		{
			pos = v.pos;
			n = v.n;
			tex = v.tex;
			boneId = v.boneId;
		}
		Vertex(DirectX::XMFLOAT3 _pos)
		{
			pos = _pos;
			n = DirectX::XMFLOAT3(0, 0, 0);
			tex = DirectX::XMFLOAT2(0, 0);
			boneId = 0;
		}
		Vertex(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 n, DirectX::XMFLOAT2 tex, DirectX::XMFLOAT3 tangent, unsigned int boneId) :pos(pos), n(n), tex(tex), boneId(boneId) {}
	};
}
