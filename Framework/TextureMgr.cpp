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
void TextureMgr::Load(std::string key, std::vector<std::string> fileNames, UINT miplevel)
{
	assert(SRVs.find(key) == SRVs.end());

	const UINT spriteCount = fileNames.size();

	std::vector<ID3D11Resource*> ori_resources(spriteCount);
	D3D11_TEXTURE2D_DESC ori_desc;
	D3D11_TEXTURE2D_DESC prev_desc;
	for (int i = 0; i < spriteCount; ++i)
	{
		ID3D11Resource* newResource = nullptr;
		std::wstring wFileName = std::wstring(fileNames[i].begin(), fileNames[i].end());
		r_assert(
			DirectX::CreateWICTextureFromFile(
				DX_Device,
				(L"Data\\Texture\\" + wFileName).c_str(),
				&newResource,
				nullptr)
		);

		ori_resources[i] = newResource;
		ID3D11Texture2D* newTex = nullptr; 
		r_assert(
			newResource->QueryInterface(IID_ID3D11Texture2D, (void**)&newTex)
		);
		newTex->GetDesc(&ori_desc);
		if (i!=0)
		{
			assert(ori_desc.Width == prev_desc.Width && ori_desc.Height == prev_desc.Height);
		}
		prev_desc = ori_desc;
	}
	
	miplevel = min(CalculateMaxMiplevel(ori_desc.Width, ori_desc.Height), miplevel);

	D3D11_TEXTURE2D_DESC arr_desc;
	arr_desc.Format = ori_desc.Format;
	arr_desc.ArraySize = spriteCount;
	arr_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	arr_desc.CPUAccessFlags = 0;
	arr_desc.Width = ori_desc.Width;
	arr_desc.Height = ori_desc.Height;
	arr_desc.MipLevels = miplevel;
	arr_desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	arr_desc.SampleDesc.Count = 1;
	arr_desc.SampleDesc.Quality = 0;
	arr_desc.Usage = D3D11_USAGE_DEFAULT;

	ComPtr<ID3D11Texture2D> arrTex;
	r_assert(
		DX_Device->CreateTexture2D(
			&arr_desc, nullptr, arrTex.GetAddressOf())
	);

	for (int i = 0; i < spriteCount; ++i)
	{
		ComPtr<ID3D11Texture2D> stagTex;
		D3D11_TEXTURE2D_DESC stagDesc = ori_desc;
		stagDesc.BindFlags = 0;
		stagDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		stagDesc.Usage = D3D11_USAGE_STAGING;
		r_assert(DX_Device->CreateTexture2D(&stagDesc, nullptr, &stagTex));

		// use staging texture to read and paste image
		// because for copying, we should match miplevels between dest and src
		DX_DContext->CopyResource(stagTex.Get(), ori_resources[i]);

		D3D11_MAPPED_SUBRESOURCE mapped;
		r_assert(DX_DContext->Map(stagTex.Get(), 0, D3D11_MAP_READ, 0, &mapped));
		// use 'UINT' because format of images from file is one of 8888
		UINT* arr = new UINT[(mapped.RowPitch/sizeof(UINT)) * ori_desc.Height];
		ZeroMemory(arr, mapped.RowPitch * ori_desc.Height);
		CopyMemory(arr, mapped.pData, mapped.RowPitch * ori_desc.Height);
		DX_DContext->Unmap(stagTex.Get(), 0);
		
		DX_DContext->UpdateSubresource(
			arrTex.Get(), D3D11CalcSubresource(0, i, miplevel), 
			nullptr, 
			arr, 
			mapped.RowPitch, mapped.DepthPitch);
		delete[] arr;
	}

	
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = ori_desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srvDesc.Texture2DArray.ArraySize = spriteCount;
	srvDesc.Texture2DArray.FirstArraySlice = 0;
	srvDesc.Texture2DArray.MipLevels = miplevel;
	srvDesc.Texture2DArray.MostDetailedMip = 0;
	ID3D11ShaderResourceView* integratedSRV = nullptr;
	r_assert(DX_Device->CreateShaderResourceView(arrTex.Get(), &srvDesc, &integratedSRV));
	DX_DContext->GenerateMips(integratedSRV);
	

	SRVs.insert(std::pair<std::string, TextureInfo>(key, TextureInfo(integratedSRV, spriteCount)));
	/*
	*/
}

void TextureMgr::Get(std::string key, ID3D11ShaderResourceView** srv, UINT* size)
{
	assert(SRVs.find(key) != SRVs.end());

	*srv = SRVs[key].srv;
	if (size)
		* size = SRVs[key].size;
}

ID3D11Texture2D* TextureMgr::GetTexture(std::string fileName)
{
	assert(SRVs.find(fileName) != SRVs.end());

	ID3D11Resource* resource=nullptr;
	SRVs[fileName].srv->GetResource(&resource);
	ID3D11Texture2D* tex=nullptr;
	r_assert( resource->QueryInterface(IID_ID3D11Texture2D, (void**)&tex) );
	return tex;
}


