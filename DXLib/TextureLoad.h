#pragma once

namespace DX {
	DXLIB_DLL void LoadTexture(ID3D11Device* device, ID3D11DeviceContext* dContext, std::string fileName, ID3D11ShaderResourceView** outSRV);
	DXLIB_DLL void LoadTextureArray(ID3D11Device* device, ID3D11DeviceContext* dContext, std::wstring folderName, std::vector<std::string> fileNames, ID3D11ShaderResourceView** outSRV);
	DXLIB_DLL void LoadTextureCM(ID3D11Device* device, ID3D11DeviceContext* dContext, const std::vector<std::string>& fileNames, ID3D11ShaderResourceView** outSRV);
}

