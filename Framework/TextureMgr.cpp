#include "TextureMgr.h"
#include <WICTextureLoader.h>


TextureMgr::~TextureMgr()
{
	for (auto& e : SRVs) {

		e.second.srv->Release();
	}
}

int CalculateMaxMiplevel(int width, int height)
{
	return log2(max(width, height));
}
void TextureMgr::Load(std::string fileName, bool mipmap, UINT spriteCount)
{
	assert(SRVs.find(fileName) == SRVs.end());

	ID3D11ShaderResourceView* newSRV = nullptr;
	ComPtr<ID3D11Resource> newResource = nullptr;

	std::wstring wFileName = std::wstring(fileName.begin(), fileName.end());

	r_assert(
		DirectX::CreateWICTextureFromFile(
			DX_Device,
			(L"Data\\Texture\\" + wFileName).c_str(),
			newResource.GetAddressOf(),
			&newSRV)
	);
	
	if (mipmap)
	{
		ComPtr<ID3D11Texture2D> oriTex = nullptr;
		r_assert(
			newResource->QueryInterface(IID_ID3D11Texture2D, (void**)oriTex.GetAddressOf())
		);

		D3D11_TEXTURE2D_DESC ori_desc;
		oriTex->GetDesc(&ori_desc);

		ComPtr<ID3D11Texture2D> mipmapTexture = nullptr;
		D3D11_TEXTURE2D_DESC mm_desc = ori_desc;
		mm_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		mm_desc.MipLevels = CalculateMaxMiplevel(ori_desc.Width, ori_desc.Height);
		mm_desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
		mm_desc.Usage = D3D11_USAGE_DEFAULT;
		mm_desc.SampleDesc = { 1,0 };
		r_assert(DX_Device->CreateTexture2D(&mm_desc, nullptr, mipmapTexture.GetAddressOf()));

		ComPtr<ID3D11Texture2D> stagTex;
		D3D11_TEXTURE2D_DESC stagDesc = ori_desc;
		stagDesc.ArraySize = 1;
		stagDesc.BindFlags = 0;
		stagDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		stagDesc.Format = ori_desc.Format;
		stagDesc.Width = ori_desc.Width;
		stagDesc.Height = ori_desc.Height;
		stagDesc.MipLevels = 1;
		stagDesc.MiscFlags = 0;
		stagDesc.Usage = D3D11_USAGE_STAGING;
		stagDesc.SampleDesc = { 1,0 };
		r_assert(DX_Device->CreateTexture2D(&stagDesc, nullptr, &stagTex));

		// use staging texture to read and paste image
		// because for copying, we should match miplevels between dest and src
		DX_DContext->CopyResource(stagTex.Get(), oriTex.Get());

		D3D11_MAPPED_SUBRESOURCE mapped;
		r_assert(DX_DContext->Map(stagTex.Get(), 0, D3D11_MAP_READ, 0, &mapped));
		// use 'UINT' because format of images from file is one of 8888
		UINT* arr = new UINT[(mapped.RowPitch / sizeof(UINT)) * ori_desc.Height];
		ZeroMemory(arr, mapped.RowPitch * ori_desc.Height);
		CopyMemory(arr, mapped.pData, mapped.RowPitch * ori_desc.Height);
		DX_DContext->Unmap(stagTex.Get(), 0);

		DX_DContext->UpdateSubresource(mipmapTexture.Get(), 0, &CD3D11_BOX(0, 0, 0, ori_desc.Width, ori_desc.Height, 1), arr, mapped.RowPitch, mapped.DepthPitch);
		delete[] arr;

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = ori_desc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = mm_desc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;
		r_assert(DX_Device->CreateShaderResourceView(mipmapTexture.Get(), &srvDesc, &newSRV));
		DX_DContext->GenerateMips(newSRV);
	}

	SRVs.insert(std::pair<std::string, SRV_Info>(fileName, SRV_Info(newSRV, spriteCount)));
}
void TextureMgr::Load(std::string fileName, UINT count)
{
	if (SRVs.find(fileName) == SRVs.end())
	{
		ID3D11Resource* oriResource = nullptr;

		std::wstring wFileName = std::wstring(fileName.begin(), fileName.end());

		r_assert(
			DirectX::CreateWICTextureFromFile(
				DX_Device,
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
			DX_Device->CreateTexture2D(
				&st_desc, nullptr, &animTex)
		);

		/*
		for (int i = 0; i < count; ++i)
		{
			DX_DContext->CopySubresourceRegion(animTex, i, 0, 0, 0, oriTex, 0, &CD3D11_BOX(i*sWidth, 0, 0, (i + 1)*sWidth, sHeight, 1));
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
			DX_Device->CreateShaderResourceView(
				animTex, &srv_desc, &animSRV)
		);

		animSRVs.insert(std::pair<std::string, AnimSRV>(fileName, AnimSRV(animSRV, count)));
		*/
	}
}

void TextureMgr::Get(std::string fileName, OUT ID3D11ShaderResourceView const ** srv, OUT UINT* count)
{
	auto iter = SRVs.find(fileName);

	assert(iter != SRVs.end());

	*srv = iter->second.srv;
	*count = iter->second.countX;
}

ID3D11Texture2D * TextureMgr::GetTexture(std::string fileName)
{
	assert(SRVs.find(fileName) != SRVs.end());

	ID3D11Resource* resource=nullptr;
	SRVs[fileName].srv->GetResource(&resource);
	ID3D11Texture2D* tex=nullptr;
	r_assert( resource->QueryInterface(IID_ID3D11Texture2D, (void**)&tex) );
	return tex;
}


