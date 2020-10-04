#pragma once


namespace DX {

class Mesh;

	DXLIB_DLL void Load(ID3D11Device* device, std::vector<Mesh*>* storage, std::string filepath, std::string filename);
}

