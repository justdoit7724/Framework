
#include "pch.h"

#include "TextureLoad.h"

#include "SDK\directxtk_desktop_2015.2019.12.17.1\include\WICTextureLoader.h"


namespace DX {

	int CalculateMaxMiplevel(int width, int height)
	{
		return fmaxf(log2(fmaxf(width, height)), 1);
	}
	void LoadTexture(ID3D11Device* device, ID3D11DeviceContext* dContext, std::string fileName, ID3D11ShaderResourceView** outSRV)
	{
		ID3D11Resource* ori_resources;
		D3D11_TEXTURE2D_DESC ori_desc;

		HRESULT hr = CreateWICTextureFromFile(
			device,
			std::wstring(fileName.begin(), fileName.end()).c_str(),
			&ori_resources,
			nullptr);
		r_assert(hr);

		ID3D11Texture2D* ori_tex = nullptr;
		hr = ori_resources->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&ori_tex);
		r_assert(hr);

		ori_tex->GetDesc(&ori_desc);

		UINT miplevel = CalculateMaxMiplevel(ori_desc.Width, ori_desc.Height);

		D3D11_TEXTURE2D_DESC newTex_desc;
		newTex_desc.Format = ori_desc.Format;
		newTex_desc.ArraySize = 1;
		newTex_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		newTex_desc.CPUAccessFlags = 0;
		newTex_desc.Width = ori_desc.Width;
		newTex_desc.Height = ori_desc.Height;
		newTex_desc.MipLevels = miplevel;
		newTex_desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
		newTex_desc.SampleDesc.Count = 1;
		newTex_desc.SampleDesc.Quality = 0;
		newTex_desc.Usage = D3D11_USAGE_DEFAULT;

		ID3D11Texture2D* newTex;
		hr = device->CreateTexture2D(
			&newTex_desc, nullptr, &newTex);
		r_assert(hr);

		ID3D11Texture2D* stagTex;
		D3D11_TEXTURE2D_DESC stagDesc = ori_desc;
		stagDesc.BindFlags = 0;
		stagDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		stagDesc.Usage = D3D11_USAGE_STAGING;
		hr = device->CreateTexture2D(&stagDesc, nullptr, &stagTex);
		r_assert(hr);

		dContext->CopyResource(stagTex, ori_tex);

		D3D11_MAPPED_SUBRESOURCE mapped;
		ZeroMemory(&mapped, sizeof(D3D11_MAPPED_SUBRESOURCE));
		hr = dContext->Map(stagTex, 0, D3D11_MAP_READ, 0, &mapped);
		r_assert(hr);
		// use 'UINT' because format of images from file is one of 8888
		UINT* arr = new UINT[(mapped.RowPitch / sizeof(UINT)) * ori_desc.Height];
		ZeroMemory(arr, mapped.RowPitch * ori_desc.Height);
		CopyMemory(arr, mapped.pData, mapped.RowPitch * ori_desc.Height);
		dContext->Unmap(stagTex, 0);

		dContext->UpdateSubresource(
			newTex, 0,
			nullptr,
			arr,
			mapped.RowPitch, mapped.DepthPitch);
		ori_resources->Release();
		ori_tex->Release();
		stagTex->Release();
		delete[] arr;



		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = ori_desc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = miplevel;
		srvDesc.Texture2D.MostDetailedMip = 0;
		hr = device->CreateShaderResourceView(newTex, &srvDesc, outSRV);
		r_assert(hr);
		dContext->GenerateMips(*outSRV);

		newTex->Release();
	}

	void LoadTextureArray(ID3D11Device* device, ID3D11DeviceContext* dContext, std::wstring folderName, std::vector<std::string> fileNames, ID3D11ShaderResourceView** outSRV)
	{
		const UINT spriteCount = fileNames.size();
		HRESULT hr;
		std::vector<ID3D11Resource*> ori_resources(spriteCount);
		D3D11_TEXTURE2D_DESC ori_desc;
		D3D11_TEXTURE2D_DESC prev_desc;
		for (int i = 0; i < spriteCount; ++i)
		{
			ID3D11Resource* newResource = nullptr;
			hr = DirectX::CreateWICTextureFromFile(
				device,
				(folderName + std::wstring(fileNames[i].begin(), fileNames[i].end())).c_str(),
				&newResource,
				nullptr);
			r_assert(hr);

			ori_resources[i] = newResource;
			ID3D11Texture2D* newTex = nullptr;
			hr = newResource->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&newTex);
			r_assert(hr);
			newTex->GetDesc(&ori_desc);
			newTex->Release();
			newResource->Release();
			if (i != 0)
			{
				assert(ori_desc.Width == prev_desc.Width && ori_desc.Height == prev_desc.Height);
			}
			prev_desc = ori_desc;
		}

		UINT miplevel = CalculateMaxMiplevel(ori_desc.Width, ori_desc.Height);

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

		ID3D11Texture2D* arrTex;
		hr = device->CreateTexture2D(
			&arr_desc, nullptr, &arrTex);
		r_assert(hr);

		ID3D11Texture2D* stagTex;
		D3D11_TEXTURE2D_DESC stagDesc = ori_desc;
		stagDesc.BindFlags = 0;
		stagDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		stagDesc.Usage = D3D11_USAGE_STAGING;
		hr = device->CreateTexture2D(&stagDesc, nullptr, &stagTex);
		r_assert(hr);
		for (int i = 0; i < spriteCount; ++i)
		{
			// use staging texture to read and paste image
			// because for copying, we should match miplevels between dest and src
			dContext->CopyResource(stagTex, ori_resources[i]);

			D3D11_MAPPED_SUBRESOURCE mapped;
			hr = dContext->Map(stagTex, 0, D3D11_MAP_READ, 0, &mapped);
			r_assert(hr);
			// use 'UINT' because format of images from file is one of 8888
			UINT* arr = new UINT[(mapped.RowPitch / sizeof(UINT)) * ori_desc.Height];
			ZeroMemory(arr, mapped.RowPitch * ori_desc.Height);
			CopyMemory(arr, mapped.pData, mapped.RowPitch * ori_desc.Height);
			dContext->Unmap(stagTex, 0);

			dContext->UpdateSubresource(
				arrTex, D3D11CalcSubresource(0, i, miplevel),
				nullptr,
				arr,
				mapped.RowPitch, mapped.DepthPitch);
			delete[] arr;
		}
		stagTex->Release();


		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = ori_desc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		srvDesc.Texture2DArray.ArraySize = spriteCount;
		srvDesc.Texture2DArray.FirstArraySlice = 0;
		srvDesc.Texture2DArray.MipLevels = miplevel;
		srvDesc.Texture2DArray.MostDetailedMip = 0;
		hr = device->CreateShaderResourceView(arrTex, &srvDesc, outSRV);
		r_assert(hr);
		dContext->GenerateMips(*outSRV);

		arrTex->Release();

	}

	void LoadTextureCM(ID3D11Device* device, ID3D11DeviceContext* dContext, const std::vector<std::string>& fileNames, ID3D11ShaderResourceView** outSRV)
	{
		HRESULT hr;
		ID3D11Resource* ori_resources[6];
		D3D11_TEXTURE2D_DESC ori_desc;
		D3D11_TEXTURE2D_DESC prev_desc;
		for (int i = 0; i < 6; ++i)
		{
			hr = DirectX::CreateWICTextureFromFile(
				device,
				std::wstring(fileNames[i].begin(), fileNames[i].end()).c_str(),
				&ori_resources[i],
				nullptr);
			r_assert(hr);

			ID3D11Texture2D* newTex = nullptr;
			hr = ori_resources[i]->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&newTex);
			r_assert(hr);
			newTex->GetDesc(&ori_desc);
			newTex->Release();
			if (i != 0)
			{
				assert(ori_desc.Width == prev_desc.Width && ori_desc.Height == prev_desc.Height);
			}
			prev_desc = ori_desc;
		}

		D3D11_TEXTURE2D_DESC cm_desc;
		cm_desc.Format = ori_desc.Format;
		cm_desc.ArraySize = 6;
		cm_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		cm_desc.CPUAccessFlags = 0;
		cm_desc.Width = ori_desc.Width;
		cm_desc.Height = ori_desc.Height;
		//debug need to modify
		cm_desc.MipLevels = 8;
		cm_desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE | D3D11_RESOURCE_MISC_GENERATE_MIPS;
		cm_desc.SampleDesc.Count = 1;
		cm_desc.SampleDesc.Quality = 0;
		cm_desc.Usage = D3D11_USAGE_DEFAULT;

		ID3D11Texture2D* cmTex;
		hr = device->CreateTexture2D(
			&cm_desc, nullptr, &cmTex);
		r_assert(hr);

		for (int i = 0; i < 6; ++i)
		{
			dContext->CopySubresourceRegion(
				cmTex, D3D11CalcSubresource(0, i, 8),
				0, 0, 0,
				ori_resources[i], 0,
				nullptr);
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = ori_desc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		srvDesc.TextureCube.MipLevels = -1;
		srvDesc.TextureCube.MostDetailedMip = 0;
		hr = device->CreateShaderResourceView(cmTex, &srvDesc, outSRV);
		r_assert(hr);
		cmTex->Release();
		for (int i = 0; i < 6; ++i)
		{
			ori_resources[i]->Release();
		}
	}
}
