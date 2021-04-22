#include "..\WinAPI\ResourceMgr.h"
#include "..\WinAPI\ResourceMgr.h"
#include "..\WinAPI\ResourceMgr.h"
#include "..\WinAPI\ResourceMgr.h"
#include "..\WinAPI\ResourceMgr.h"
#include "..\WinAPI\ResourceMgr.h"
#include "pch.h"
#include "ResourceMgr.h"
#include "../Packages/directxtk_desktop_2015.2019.12.17.1/include/WICTextureLoader.h"
#include "Shader.h"
#include "Buffer.h"




ResourceMgr::ResourceMgr(ID3D11Device* device, ID3D11DeviceContext* dContext)
	:m_device(device), m_dContext(dContext)
{
}

ResourceMgr::~ResourceMgr()
{
	for (auto& srv : m_mSRV)
		srv.second->Release();
	for (auto& shader : m_mVShader)
		delete shader.second;
	for (auto& shader : m_mPShader)
		delete shader.second;
}
int CalculateMaxMiplevel(int width, int height)
{
	return fmaxf(log2(fmaxf(width, height)), 1);
}

std::string GetTitle(std::string path)
{
	std::string tmpPath = path;
	
	//removing path
	while (true)
	{
		size_t slashPos = tmpPath.find('\\');
		if (slashPos == std::string::npos)
			break;
		tmpPath = tmpPath.substr(slashPos + 1);
	}
	while (true)
	{
		size_t slashPos = tmpPath.find('/');
		if (slashPos == std::string::npos)
			break;
		tmpPath = tmpPath.substr(slashPos + 1);
	}

	//removing extension
	size_t extPos = path.find('.');
	if (extPos != std::string::npos)
	{
		tmpPath = tmpPath.substr(0, extPos);
	}

	return tmpPath;
}

BOOL ResourceMgr::LoadTexture(std::string fileName)
{
	std::string title = GetTitle(fileName);

	if (HasTexture(title))
	{
		m_mSRV[title]->Release();
	}

	ID3D11Resource* ori_resources;
	D3D11_TEXTURE2D_DESC ori_desc;

	HRESULT hr = CreateWICTextureFromFile(
		m_device,
		std::wstring(fileName.begin(), fileName.end()).c_str(),
		&ori_resources,
		nullptr);
	if (FAILED(hr))
		return FALSE;

	ID3D11Texture2D* ori_tex = nullptr;
	hr = ori_resources->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&ori_tex);
	if (FAILED(hr))
		return FALSE;

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
	hr = m_device->CreateTexture2D(
		&newTex_desc, nullptr, &newTex);
	if (FAILED(hr))
		return FALSE;

	ID3D11Texture2D* stagTex;
	D3D11_TEXTURE2D_DESC stagDesc = ori_desc;
	stagDesc.BindFlags = 0;
	stagDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	stagDesc.Usage = D3D11_USAGE_STAGING;
	hr = m_device->CreateTexture2D(&stagDesc, nullptr, &stagTex);
	if (FAILED(hr))
		return FALSE;

	m_dContext->CopyResource(stagTex, ori_tex);

	D3D11_MAPPED_SUBRESOURCE mapped;
	ZeroMemory(&mapped, sizeof(D3D11_MAPPED_SUBRESOURCE));
	hr = m_dContext->Map(stagTex, 0, D3D11_MAP_READ, 0, &mapped);
	if (FAILED(hr))
		return FALSE;
	// use 'UINT' because format of images from file is one of 8888
	UINT* arr = new UINT[(mapped.RowPitch / sizeof(UINT)) * ori_desc.Height];
	ZeroMemory(arr, mapped.RowPitch * ori_desc.Height);
	CopyMemory(arr, mapped.pData, mapped.RowPitch * ori_desc.Height);
	m_dContext->Unmap(stagTex, 0);

	m_dContext->UpdateSubresource(
		newTex, 0,
		nullptr,
		arr,
		mapped.RowPitch, mapped.DepthPitch);
	ori_resources->Release();
	ori_tex->Release();
	stagTex->Release();
	delete[] arr;


	ID3D11ShaderResourceView* srv;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = ori_desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = miplevel;
	srvDesc.Texture2D.MostDetailedMip = 0;
	hr = m_device->CreateShaderResourceView(newTex, &srvDesc, &srv);
	if (FAILED(hr))
		return FALSE;
	m_dContext->GenerateMips(srv);

	newTex->Release();

	m_mSRV[title] = srv;

	return TRUE;
}

BOOL ResourceMgr::LoadTextureArray(std::wstring folderName, std::vector<std::string> fileNames, std::string key)
{
	if (HasTexture(key))
	{
		m_mSRV[key]->Release();
	}

	const UINT spriteCount = fileNames.size();
	HRESULT hr;
	std::vector<ID3D11Resource*> ori_resources(spriteCount);
	D3D11_TEXTURE2D_DESC ori_desc;
	D3D11_TEXTURE2D_DESC prev_desc;
	for (int i = 0; i < spriteCount; ++i)
	{
		ID3D11Resource* newResource = nullptr;
		hr = DirectX::CreateWICTextureFromFile(
			m_device,
			(folderName + std::wstring(fileNames[i].begin(), fileNames[i].end())).c_str(),
			&newResource,
			nullptr);
		if (FAILED(hr))
			return FALSE;

		ori_resources[i] = newResource;
		ID3D11Texture2D* newTex = nullptr;
		hr = newResource->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&newTex);
		if (FAILED(hr))
			return FALSE;
		newTex->GetDesc(&ori_desc);
		newTex->Release();
		newResource->Release();
		if (i != 0)
		{
			if (ori_desc.Width != prev_desc.Width || ori_desc.Height != prev_desc.Height)
				return FALSE;
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
	hr = m_device->CreateTexture2D(
		&arr_desc, nullptr, &arrTex);
	if (FAILED(hr))
		return FALSE;

	ID3D11Texture2D* stagTex;
	D3D11_TEXTURE2D_DESC stagDesc = ori_desc;
	stagDesc.BindFlags = 0;
	stagDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	stagDesc.Usage = D3D11_USAGE_STAGING;
	hr = m_device->CreateTexture2D(&stagDesc, nullptr, &stagTex);
	if (FAILED(hr))
		return FALSE;
	for (int i = 0; i < spriteCount; ++i)
	{
		// use staging texture to read and paste image
		// because for copying, we should match miplevels between dest and src
		m_dContext->CopyResource(stagTex, ori_resources[i]);

		D3D11_MAPPED_SUBRESOURCE mapped;
		hr = m_dContext->Map(stagTex, 0, D3D11_MAP_READ, 0, &mapped);
		if (FAILED(hr))
			return FALSE;
		// use 'UINT' because format of images from file is one of 8888
		UINT* arr = new UINT[(mapped.RowPitch / sizeof(UINT)) * ori_desc.Height];
		ZeroMemory(arr, mapped.RowPitch * ori_desc.Height);
		CopyMemory(arr, mapped.pData, mapped.RowPitch * ori_desc.Height);
		m_dContext->Unmap(stagTex, 0);

		m_dContext->UpdateSubresource(
			arrTex, D3D11CalcSubresource(0, i, miplevel),
			nullptr,
			arr,
			mapped.RowPitch, mapped.DepthPitch);
		delete[] arr;
	}
	stagTex->Release();

	ID3D11ShaderResourceView* srv;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = ori_desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srvDesc.Texture2DArray.ArraySize = spriteCount;
	srvDesc.Texture2DArray.FirstArraySlice = 0;
	srvDesc.Texture2DArray.MipLevels = miplevel;
	srvDesc.Texture2DArray.MostDetailedMip = 0;
	hr = m_device->CreateShaderResourceView(arrTex, &srvDesc, &srv);
	if (FAILED(hr))
		return FALSE;
	m_dContext->GenerateMips(srv);

	arrTex->Release();

	m_mSRV[key] = srv;

	return TRUE;
}

BOOL ResourceMgr::LoadTextureCM(const std::vector<std::string>& fileNames, std::string key)
{
	if (HasTexture(key))
	{
		m_mSRV[key]->Release();
	}

	HRESULT hr;
	ID3D11Resource* ori_resources[6];
	D3D11_TEXTURE2D_DESC ori_desc;
	D3D11_TEXTURE2D_DESC prev_desc;
	for (int i = 0; i < 6; ++i)
	{
		hr = DirectX::CreateWICTextureFromFile(
			m_device,
			std::wstring(fileNames[i].begin(), fileNames[i].end()).c_str(),
			&ori_resources[i],
			nullptr);
		if (FAILED(hr))
			return FALSE;

		ID3D11Texture2D* newTex = nullptr;
		hr = ori_resources[i]->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&newTex);
		if (FAILED(hr))
			return FALSE;
		newTex->GetDesc(&ori_desc);
		newTex->Release();
		if (i != 0)
		{
			if (ori_desc.Width != prev_desc.Width || ori_desc.Height != prev_desc.Height)
			{
				return FALSE;
			}
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
	hr = m_device->CreateTexture2D(
		&cm_desc, nullptr, &cmTex);
	if (FAILED(hr))
		return FALSE;

	for (int i = 0; i < 6; ++i)
	{
		m_dContext->CopySubresourceRegion(
			cmTex, D3D11CalcSubresource(0, i, 8),
			0, 0, 0,
			ori_resources[i], 0,
			nullptr);
	}

	ID3D11ShaderResourceView* srv;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = ori_desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	srvDesc.TextureCube.MipLevels = -1;
	srvDesc.TextureCube.MostDetailedMip = 0;
	hr = m_device->CreateShaderResourceView(cmTex, &srvDesc, &srv);
	if (FAILED(hr))
		return FALSE;
	cmTex->Release();
	for (int i = 0; i < 6; ++i)
	{
		ori_resources[i]->Release();
	}

	m_mSRV[key] = srv;

	return TRUE;
}

BOOL ResourceMgr::GetTexture(std::string name, ID3D11ShaderResourceView** srv)
{
	if (!HasTexture(name))
		return 1;

	*srv = m_mSRV[name];

	return TRUE;
}

BOOL ResourceMgr::HasTexture(std::string name)
{
	return (m_mSRV.find(name) != m_mSRV.end());
}

BOOL ResourceMgr::LoadVShader(std::string csoPath)
{
	std::string title = GetTitle(csoPath);

	if (HasVShader(title))
	{
		delete m_mVShader[title];
	}

	VShader* newVShader = new VShader;
	if (!newVShader->Modify(m_device, csoPath))
	{
		delete newVShader;
		return FALSE;
	}
	
	m_mVShader[title] = newVShader;

	return TRUE;
}

BOOL ResourceMgr::GetVShader(std::string title, VShader** shader)
{
	if (!HasVShader(title))
		return FALSE;

	*shader = m_mVShader[title];

	return TRUE;
}

BOOL ResourceMgr::HasVShader(std::string key)
{
	return (m_mVShader.find(key)!= m_mVShader.end());
}

BOOL ResourceMgr::LoadPShader(std::string csoPath)
{
	std::string name = GetTitle(csoPath);

	if (HasPShader(name))
	{
		delete m_mPShader[name];
	}

	PShader* newPShader = new PShader;
	if (!newPShader->Modify(m_device, csoPath))
	{
		delete newPShader;
		return FALSE;
	}
	m_mPShader[name] = newPShader;

	return TRUE;
}

BOOL ResourceMgr::GetPShader(std::string title, PShader** shader)
{
	if (!HasPShader(title))
	{
		return FALSE;
	}

	*shader = m_mPShader[title];

	return TRUE;
}

BOOL ResourceMgr::HasPShader(std::string key)
{
	return (m_mPShader.find(key) != m_mPShader.end());
}

BOOL ResourceMgr::GetMesh(std::string title, VertexBuffer** vb, IndexBuffer** ib)
{
	*vb = m_mMeshBuffer[title].vb;
	*ib = m_mMeshBuffer[title].ib;

	return TRUE;
}

BOOL ResourceMgr::LoadMesh(const void* vertice, int verticeCount, int vertexByteStride, const UINT* indice, int indiceCount, std::string key)
{
	if (HasMesh(key))
	{
		delete m_mMeshBuffer[key].vb;
		delete m_mMeshBuffer[key].ib;
	}

	MeshInfo newMesh;
	newMesh.vb = new VertexBuffer(m_device, vertice, verticeCount, vertexByteStride);
	newMesh.ib = new IndexBuffer(m_device, indice, indiceCount);
	m_mMeshBuffer[key] = newMesh;

	return TRUE;
}

BOOL ResourceMgr::HasMesh(std::string key)
{
	return (m_mMeshBuffer.find(key)!= m_mMeshBuffer.end());
}


