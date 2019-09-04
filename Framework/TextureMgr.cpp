#include "TextureMgr.h"
#include <WICTextureLoader.h>
#include "Network.h"

TextureMgr::~TextureMgr()
{
	for (auto& tex : textures) {

		tex.second->Release();
	}
}


void TextureMgr::Load(IGraphic* graphic, std::string fileName)
{
	if (textures.find(fileName) == textures.end())
	{
		ID3D11ShaderResourceView* newSRV = nullptr;

		std::wstring wFileName = std::wstring(fileName.begin(), fileName.end());

		r_assert(
			DirectX::CreateWICTextureFromFile(
				graphic->Device(),
				(L"Data\\Texture\\" + wFileName).c_str(),
				nullptr,
				&newSRV)
		);

		textures.insert(std::pair<std::string, ID3D11ShaderResourceView*>(fileName, newSRV));
	}
}

ID3D11ShaderResourceView* TextureMgr::Get(std::string fileName)
{
	auto iter = textures.find(fileName);

	assert(iter != textures.end());
	
	return iter->second;
}

ID3D11Texture2D * TextureMgr::GetTexture(std::string fileName)
{
	assert(textures.find(fileName) != textures.end());

	ID3D11Resource* resource=nullptr;
	textures[fileName]->GetResource(&resource);
	ID3D11Texture2D* tex=nullptr;
	r_assert( resource->QueryInterface(IID_ID3D11Texture2D, (void**)&tex) );
	return tex;
}


