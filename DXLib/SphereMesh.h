#pragma once

#include "Mesh.h"

namespace DX {

	class VertexLayout;

	class DXLIB_DLL SphereMesh :
		public Mesh
	{
	public:
		SphereMesh(ID3D11Device* device, const int numSubDivision, const VertexLayout* layout);
	};
}

