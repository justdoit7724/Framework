#pragma once
#include "Mesh.h"

namespace DX {

	class VertexLayout;

	class DXLIB_DLL CubeMesh :
		public Mesh
	{
	public:
		CubeMesh(ID3D11Device* device, const VertexLayout* layout);
	};

}
