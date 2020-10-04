#pragma once

#include "Mesh.h"

namespace DX {
	class DXLIB_DLL QuadMesh :
		public Mesh
	{
	public:
		QuadMesh(ID3D11Device* device);
	};
}

