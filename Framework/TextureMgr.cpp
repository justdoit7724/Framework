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
void TextureMgr::Load(std::string fileName, UINT miplevel, UINT spriteCount)
{
	assert(SRVs.find(fileName) == SRVs.end());


	ComPtr<ID3D11Resource> newResource = nullptr;
	std::wstring wFileName = std::wstring(fileName.begin(), fileName.end());
	r_assert(
		DirectX::CreateWICTextureFromFile(
			DX_Device,
			(L"Data\\Texture\\" + wFileName).c_str(),
			newResource.GetAddressOf(),
			nullptr)
	);


	ComPtr<ID3D11Texture2D> oriTex = nullptr;
	r_assert(
		newResource->QueryInterface(IID_ID3D11Texture2D, (void**)oriTex.GetAddressOf())
	);

	D3D11_TEXTURE2D_DESC ori_desc;
	oriTex->GetDesc(&ori_desc);

	miplevel = min(CalculateMaxMiplevel(ori_desc.Width, ori_desc.Height), miplevel);
	UINT sWidth = ori_desc.Width / (float)spriteCount;
	UINT sHeight = ori_desc.Height;

	D3D11_TEXTURE2D_DESC arr_desc;
	arr_desc.Format = ori_desc.Format;
	arr_desc.ArraySize = spriteCount;
	arr_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE| D3D11_BIND_RENDER_TARGET;
	arr_desc.CPUAccessFlags = 0;
	arr_desc.Width = sWidth;
	arr_desc.Height = sHeight;
	arr_desc.MipLevels = 1;
	arr_desc.MiscFlags = 0;
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
		DX_DContext->CopySubresourceRegion(arrTex.Get(), i, 0, 0, 0, oriTex.Get(), 0, &CD3D11_BOX(i*sWidth, 0, 0, (i + 1)*sWidth, sHeight, 1));
	}


	ComPtr<ID3D11Texture2D> stagTex;
	D3D11_TEXTURE2D_DESC stagDesc = arr_desc;
	stagDesc.Width = ori_desc.Width;
	stagDesc.Height = ori_desc.Height;
	stagDesc.ArraySize = 1;
	stagDesc.BindFlags = 0;
	stagDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	stagDesc.Usage = D3D11_USAGE_STAGING;
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

	for (int i = 0; i < spriteCount; ++i)
	{
		DX_DContext->UpdateSubresource(arrTex.Get(), i, &CD3D11_BOX(0, 0, 0, arr_desc.Width, arr_desc.Height, 1), arr, sizeof(UINT)*arr_desc.Width, mapped.DepthPitch);
	}
	delete[] arr;
	/*

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = ori_desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = mm_desc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	r_assert(DX_Device->CreateShaderResourceView(mipmapTexture.Get(), &srvDesc, &newSRV));
	DX_DContext->GenerateMips(newSRV);
	

	SRVs.insert(std::pair<std::string, SRV_Info>(fileName, SRV_Info(newSRV, spriteCount)));
	*/
}

void TextureMgr::Get(std::string fileName, OUT ComPtr<ID3D11ShaderResourceView>* srv, OUT UINT* count)
{
	auto iter = SRVs.find(fileName);

	assert(iter != SRVs.end());

	*srv = iter->second.srv;
	if(count)
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


