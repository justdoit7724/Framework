
#include "pch.h"

#include "Shader.h"
#pragma comment(lib, "D3DCompiler.lib")

static const int g_ILayoutCount = 4;
static const D3D11_INPUT_ELEMENT_DESC g_StdILayouts[g_ILayoutCount] =
{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMFLOAT3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(XMFLOAT3) + sizeof(XMFLOAT3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BONEINDICE", 0, DXGI_FORMAT_R8G8B8A8_UINT, 0, sizeof(XMFLOAT3) + sizeof(XMFLOAT3) + sizeof(XMFLOAT2), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,sizeof(XMFLOAT3) * 2 + sizeof(XMFLOAT2), D3D11_INPUT_PER_VERTEX_DATA,0}
};

VShader::VShader()
	:vs(nullptr),
	iLayout(nullptr)
{
	

}

VShader::~VShader()
{
	if(iLayout)
		iLayout->Release();
	if(vs)
		vs->Release();
}


BOOL VShader::Modify(ID3D11Device* device, std::string fileName)
{
	if (iLayout)
	{
		iLayout->Release();
		vs->Release();
	}

	std::wstring wVS(fileName.begin(), fileName.end());
	ID3DBlob* vsBlob;
	HRESULT hr = D3DReadFileToBlob(
		wVS.c_str(),
		&vsBlob);
	if (FAILED(hr))
		return FALSE;
	hr = device->CreateVertexShader(
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		nullptr,
		&vs);
	if (FAILED(hr))
		return FALSE;

	hr = device->CreateInputLayout(
		g_StdILayouts,
		g_ILayoutCount,
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		&iLayout);
	if (FAILED(hr))
		return FALSE;
	vsBlob->Release();

	return TRUE;
}

BOOL VShader::Apply(ID3D11DeviceContext* dContext)const
{
	dContext->IASetInputLayout(iLayout);
	dContext->VSSetShader(vs, nullptr, 0);

	return TRUE;
}

GShader::GShader()
	:gs(nullptr)
{
}

GShader::~GShader()
{
	if(gs)
		gs->Release();
}

BOOL GShader::Modify(ID3D11Device* device, std::string csoPath)
{
	if (gs)
	{
		gs->Release();
	}

	std::wstring wGS(csoPath.begin(), csoPath.end());
	ID3DBlob* blob;

	HRESULT hr = D3DReadFileToBlob(
		wGS.c_str(),
		&blob);
	if (FAILED(hr))
		return FALSE;
	hr = device->CreateGeometryShader(
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		nullptr,
		&gs);
	if (FAILED(hr))
		return FALSE;

	return TRUE;
}

BOOL GShader::Apply(ID3D11DeviceContext* dContext)const
{
	dContext->GSSetShader(gs, nullptr, 0);

	return TRUE;
}

PShader::PShader()
	:ps(nullptr)
{
}
PShader::~PShader()
{
	if(ps)
		ps->Release();
}
BOOL PShader::Modify(ID3D11Device* device, std::string csoPath)
{
	if (ps)
	{
		ps->Release();
	}

	std::wstring wPS(csoPath.begin(), csoPath.end());
	ID3DBlob* psBlob;
	HRESULT hr = D3DReadFileToBlob(
		wPS.c_str(),
		&psBlob);
	if (FAILED(hr))
		return FALSE;
	hr = device->CreatePixelShader(
		psBlob->GetBufferPointer(),
		psBlob->GetBufferSize(),
		nullptr,
		&ps);
	if (FAILED(hr))
		return FALSE;
	psBlob->Release();

	return TRUE;
}
BOOL PShader::Apply(ID3D11DeviceContext* dContext)const
{
	dContext->PSSetShader(ps, nullptr, 0);

	return TRUE;
}

CShader::CShader()
	:cs(nullptr)
{
	
}

CShader::~CShader()
{
	if(cs)
		cs->Release();
}

BOOL CShader::Modify(ID3D11Device* device, std::string csoPath)
{
	if (cs)
	{
		cs->Release();
	}

	std::wstring wCS(csoPath.begin(), csoPath.end());
	ID3DBlob* csBlob;

	HRESULT hr = D3DReadFileToBlob(
		wCS.c_str(),
		&csBlob);
	if (FAILED(hr))
		return FALSE;
	hr = device->CreateComputeShader(
		csBlob->GetBufferPointer(),
		csBlob->GetBufferSize(),
		nullptr,
		&cs);
	if (FAILED(hr))
		return FALSE;
	csBlob->Release();

	return TRUE;
}

BOOL CShader::Apply(ID3D11DeviceContext* dContext)const
{
	dContext->CSSetShader(cs, nullptr, 0);

	return TRUE;
}

HShader::HShader()
	:hs(nullptr)
{
	
}

HShader::~HShader()
{
	if(hs)
		hs->Release();
}

BOOL HShader::Modify(ID3D11Device* device, std::string csoPath)
{
	std::wstring wCS(csoPath.begin(), csoPath.end());
	ID3DBlob* blob;

	HRESULT hr = D3DReadFileToBlob(
		wCS.c_str(),
		&blob);
	if (FAILED(hr))
		return FALSE;

	hr = device->CreateHullShader(
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		nullptr,
		&hs);
	if (FAILED(hr))
		return FALSE;

	return TRUE;
}

BOOL HShader::Apply(ID3D11DeviceContext* dContext)const
{
	dContext->HSSetShader(hs, nullptr, 0);

	return TRUE;
}

DShader::DShader()
	:ds(nullptr)
{
}

DShader::~DShader()
{
	if(ds)
		ds->Release();
}

BOOL DShader::Modify(ID3D11Device* device, std::string csoPath)
{
	std::wstring wCS(csoPath.begin(), csoPath.end());
	ID3DBlob* blob;

	HRESULT hr = D3DReadFileToBlob(
		wCS.c_str(),
		&blob);
	if (FAILED(hr))
		return FALSE;

	hr = device->CreateDomainShader(
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		nullptr,
		&ds);
	if (FAILED(hr))
		return FALSE;
	blob->Release();

	return TRUE;
}

BOOL DShader::Apply(ID3D11DeviceContext* dContext)const
{
	dContext->DSSetShader(ds, nullptr, 0);

	return TRUE;
}