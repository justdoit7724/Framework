#pragma once


namespace DX {

	class SkinnedData;

	DXLIB_DLL void LoadOBJ(ID3D11Device* device, SkinnedData* storage, std::string filepath, std::string filename);
}

