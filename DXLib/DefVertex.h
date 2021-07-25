#pragma once

#include <DirectXMath.h>

namespace DX
{
	struct Vertex {
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 n;
		DirectX::XMFLOAT2 tex;
		unsigned char boneId[4];
		//XMFLOAT3 tangent;

		Vertex() :pos(DirectX::XMFLOAT3(0, 0, 0)), n(DirectX::XMFLOAT3(0, 0, 0)), tex(DirectX::XMFLOAT2(0, 0)) 
		{
			boneId[0] = -1;
			boneId[1] = -1;
			boneId[2] = -1;
			boneId[3] = -1;
		}
		Vertex(const Vertex& v)
		{
			pos = v.pos;
			n = v.n;
			tex = v.tex;
			boneId[0] = v.boneId[0];
			boneId[1] = v.boneId[1];
			boneId[2] = v.boneId[2];
			boneId[3] = v.boneId[3];
		}
		Vertex(DirectX::XMFLOAT3 _pos)
		{
			pos = _pos;
			n = DirectX::XMFLOAT3(0, 0, 0);
			tex = DirectX::XMFLOAT2(0, 0);
			boneId[0] = -1;
			boneId[1] = -1;
			boneId[2] = -1;
			boneId[3] = -1;
		}
		Vertex(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 n, DirectX::XMFLOAT2 tex, DirectX::XMFLOAT3 tangent, unsigned char boneId[4])
			:pos(pos), n(n), tex(tex)
		{
			this->boneId[0] = boneId[0];
			this->boneId[1] = boneId[1];
			this->boneId[2] = boneId[2];
			this->boneId[3] = boneId[3];
		}
	};
}
