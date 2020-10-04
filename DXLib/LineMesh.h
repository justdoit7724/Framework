#pragma once

#include "Mesh.h"

namespace DX {
	class DXLIB_DLL LineMesh : public Mesh
	{
	public:
		LineMesh();

		void Add(XMFLOAT3 a, XMFLOAT3 b);
		void Generate(ID3D11Device* device);
		void Clear();
		void Apply(ID3D11DeviceContext* dContext) const override;

	private:

		bool isGenerated = false;

		struct LineInfo
		{
			XMFLOAT3 a, b;
		};

		std::vector<LineInfo> lines;
	};
}

