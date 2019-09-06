#include "TextureMgr.h"
#include <WICTextureLoader.h>
#include "Network.h"

TextureMgr::~TextureMgr()
{
	for (auto& e : SRVs) {

		e.second->Release();
	}
	for (auto& e : animSRVs) {

		e.second.srv->Release();
	}
}


void TextureMgr::Load(IGraphic* graphic, std::string fileName)
{
	if (SRVs.find(fileName) == SRVs.end())
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

		SRVs.insert(std::pair<std::string, ID3D11ShaderResourceView*>(fileName, newSRV));
	}
}
void TextureMgr::Load(IGraphic * graphic, std::string fileName, UINT count)
{
	if (animSRVs.find(fileName) == animSRVs.end())
	{
		ID3D11Resource* oriResource = nullptr;

		std::wstring wFileName = std::wstring(fileName.begin(), fileName.end());

		r_assert(
			DirectX::CreateWICTextureFromFile(
				graphic->Device(),
				(L"Data\\Texture\\" + wFileName).c_str(),
				&oriResource,
				nullptr)
		);
		ID3D11Texture2D* oriTex = nullptr;
		oriResource->QueryInterface(IID_ID3D11Texture2D, (void**)&oriTex);
		D3D11_TEXTURE2D_DESC st_desc;
		oriTex->GetDesc(&st_desc);
		float sWidth = st_desc.Width / count;
		float sHeight = st_desc.Height;
		
		st_desc.Format = st_desc.Format;
		st_desc.ArraySize = count;
		st_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		st_desc.CPUAccessFlags = 0;
		st_desc.Width = sWidth;
		st_desc.Height = sHeight;
		st_desc.MipLevels = 1;
		st_desc.MiscFlags = 0;
		st_desc.SampleDesc.Count = 1;
		st_desc.SampleDesc.Quality = 0;
		st_desc.Usage = D3D11_USAGE_DEFAULT;

		ID3D11Texture2D* animTex;
		r_assert(
			graphic->Device()->CreateTexture2D(
				&st_desc, nullptr, &animTex)
		);

		for (int i = 0; i < count; ++i)
		{
			graphic->DContext()->CopySubresourceRegion(animTex, i, 0, 0, 0, oriTex, 0, &CD3D11_BOX(i*sWidth, 0, 0, (i + 1)*sWidth, sHeight, 1));
		}

		ID3D11ShaderResourceView* animSRV;
		D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
		srv_desc.Format = st_desc.Format;
		srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		srv_desc.Texture2DArray.ArraySize = count;
		srv_desc.Texture2DArray.FirstArraySlice = 0;
		srv_desc.Texture2DArray.MipLevels = 1;
		srv_desc.Texture2DArray.MostDetailedMip = 0;
		r_assert(
			graphic->Device()->CreateShaderResourceView(
				animTex, &srv_desc, &animSRV)
		);

		animSRVs.insert(std::pair<std::string, AnimSRV>(fileName, AnimSRV(animSRV, count)));
	}
}

ID3D11ShaderResourceView * TextureMgr::Get(std::string fileName)
{
	auto iter = SRVs.find(fileName);

	assert(iter != SRVs.end());
	
	return iter->second;
}

ID3D11ShaderResourceView *const* TextureMgr::GetAddress(std::string fileName)
{
	auto iter = SRVs.find(fileName);

	assert(iter != SRVs.end());

	return &(iter->second);
}

void TextureMgr::Get(std::string fileName, OUT ID3D11ShaderResourceView const ** srv, OUT UINT* count)
{
	auto iter = animSRVs.find(fileName);

	assert(iter != animSRVs.end());

	*srv = iter->second.srv;
	*count = iter->second.countX;
}

ID3D11Texture2D * TextureMgr::GetTexture(std::string fileName)
{
	assert(SRVs.find(fileName) != SRVs.end());

	ID3D11Resource* resource=nullptr;
	SRVs[fileName]->GetResource(&resource);
	ID3D11Texture2D* tex=nullptr;
	r_assert( resource->QueryInterface(IID_ID3D11Texture2D, (void**)&tex) );
	return tex;
}


