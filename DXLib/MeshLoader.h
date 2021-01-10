#pragma once


namespace DX {

	class Mesh;

	DXLIB_DLL void LoadOBJ(ID3D11Device* device, std::vector<Mesh*>* storage, std::string filepath, std::string filename);
}

