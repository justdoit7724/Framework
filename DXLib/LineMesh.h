#pragma once

#include "Mesh.h"

namespace DX {
	class LineMesh : public Mesh
	{
	public:
		LineMesh();

		void Add(XMFLOAT3 a, XMFLOAT3 b);
		void Generate();
		void Clear();
		void Apply() const override;

	private:

		bool isGenerated = false;

		struct LineInfo
		{
			XMFLOAT3 a, b;
		};

		std::vector<LineInfo> lines;
	};
}

