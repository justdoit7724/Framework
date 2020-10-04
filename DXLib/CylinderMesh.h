#pragma once

#include "Mesh.h"

namespace DX
{
	class DXLIB_DLL CylinderMesh : public Mesh
	{
	public:
		CylinderMesh(ID3D11Device* device, int sCount);
	};
}