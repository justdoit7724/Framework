#pragma once

#include "Mesh.h"

namespace DX {
	class DXLIB_DLL SphereMesh :
		public Mesh
	{
	public:
		SphereMesh(ID3D11Device* device, const int numSubDivision);
	};
}

