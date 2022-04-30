#pragma once

#include "Mesh.h"

namespace DX {
	class VertexLayout;

	class DXLIB_DLL QuadMesh :
		public Mesh
	{
	public:
		QuadMesh(ID3D11Device* device, const VertexLayout* layout);
	};
}

