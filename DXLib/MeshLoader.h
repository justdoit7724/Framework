#pragma once


namespace DX {

class Mesh;

	inline void DXLIB_DLL Load(ID3D11Device* device, std::vector<Mesh*>* storage, std::string filepath, std::string filename);
}

