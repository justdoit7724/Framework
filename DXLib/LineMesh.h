#pragma once

#include "Mesh.h"

namespace DX {

	class VertexLayout;

	class DXLIB_DLL LineMesh : public Mesh
	{
	public:
		LineMesh();

		void Add(XMFLOAT3 a, XMFLOAT3 b);
<<<<<<< HEAD
		void Generate(ID3D11Device* device, UINT id);
=======
		void Generate(ID3D11Device* device, const VertexLayout* layout);
>>>>>>> 03_DepthPeeling
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

