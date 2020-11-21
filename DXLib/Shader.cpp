
#include "pch.h"

#include "Shader.h"
#include "Buffer.h"
#pragma comment(lib, "D3DCompiler.lib")

using namespace DX;


static const wchar_t* ShaderPath() {
#ifdef _DEBUG
#ifdef _WIN64
	return L"..\\x64\\Debug\\";
#else
	return L"..\\Debug\\";
#endif
#else
#ifdef _WIN64
	return L"..\\x64\\Release\\";
#else
	return L"..\\Release\\";
#endif 
#endif
}

Shader::~Shader()
{
	for (auto cb : cbs)
	{
		delete cb.second.data;
	}
	for (auto samp : samps)
	{
		if(samp.second.data)
			samp.second.data->Release();
	}
}

void Shader::AddCB(ID3D11Device* device, UINT slot, UINT arrayNum, UINT byteSize)
{
	if (cbs.find(slot) == cbs.end())
	{
		cbs.insert(std::pair<UINT, BindingCB>(slot, BindingCB(new Buffer(device,byteSize), arrayNum)));
	}
	else
	{
		// slot overlap
		assert(false);
	}
}

void Shader::AddSRV(UINT slot, UINT arrayNum)
{
	assert(srvs.find(slot) == srvs.end());

	srvs.insert(std::pair<UINT, BindingSRV>(slot, BindingSRV(nullptr, arrayNum)));
}

void Shader::AddSamp(ID3D11Device* device, UINT slot, UINT arrayNum, D3D11_SAMPLER_DESC * desc)
{
	if (samps.find(slot) == samps.end())
	{
		ID3D11SamplerState* newSampler=nullptr;
		device->CreateSamplerState(desc, &newSampler);

		samps.insert(std::pair<UINT, BindingSamp>(slot, BindingSamp(newSampler, arrayNum)));
	}
	else
	{
		// slot overlap
		assert(false);
	}
}

void Shader::WriteCB(ID3D11DeviceContext* dContext, UINT slot, const void * data)
{
	if (cbs.find(slot)!=cbs.end() && data)
	{
		cbs[slot].data->Write(dContext, data);
	}
	else
	{
		//no reg
		assert(false);
	}
}

void Shader::WriteSRV(UINT slot, ID3D11ShaderResourceView* srv)
{
	assert(srvs.find(slot) != srvs.end());

	srvs[slot].data = srv;
}

void Shader::RemoveCB(UINT slot)
{
	assert(cbs.find(slot) != cbs.end());

	cbs.erase(slot);
}

bool Shader::CheckCBSlot(UINT slot)
{
	return cbs.count(slot);
}




VShader::VShader(ID3D11Device* device, std::string fileName, const D3D11_INPUT_ELEMENT_DESC * layoutDesc, UINT layoutNum)
{
	std::wstring wVS(fileName.begin(), fileName.end());
	ID3DBlob* vsBlob;
	HRESULT hr = D3DReadFileToBlob(
		(ShaderPath() + wVS).c_str(),
			&vsBlob);
	r_assert(hr);
	hr = device->CreateVertexShader(
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		nullptr,
		&vs);
	r_assert(hr);

	hr = device->CreateInputLayout(
			layoutDesc,
			layoutNum,
			vsBlob->GetBufferPointer(),
			vsBlob->GetBufferSize(),
			&iLayout);
	r_assert(hr);
	vsBlob->Release();

}

VShader::~VShader()
{
	if(iLayout)
		iLayout->Release();
	if(vs)
		vs->Release();
}


void VShader::Apply(ID3D11DeviceContext* dContext)const
{
	dContext->IASetInputLayout(iLayout);
	dContext->VSSetShader(vs, nullptr, 0);

	for (auto i = cbs.begin(); i != cbs.end(); ++i)
	{
		dContext->VSSetConstantBuffers(i->first, i->second.arrayNum, i->second.data->GetAddress());
	}
	for (auto i = srvs.begin(); i != srvs.end(); ++i)
	{
		UINT slot = i->first;
		UINT arrayNum = i->second.arrayNum;
		ID3D11ShaderResourceView* srv = i->second.data;

		dContext->VSSetShaderResources(slot, arrayNum, &srv);
	}
	for (auto i = samps.begin(); i != samps.end(); ++i)
	{
		UINT slot = i->first;
		UINT arrayNum = i->second.arrayNum;
		ID3D11SamplerState* samp = i->second.data;

		dContext->VSSetSamplers(slot, arrayNum, &samp);
	}
}


GShader::GShader(ID3D11Device* device, std::string fileName)
{
	if (fileName != "")
	{
		std::wstring wGS(fileName.begin(), fileName.end());
		ID3DBlob* blob;

		HRESULT hr = D3DReadFileToBlob(
			(ShaderPath() + wGS).c_str(),
				&blob);
		r_assert(hr);
		hr = device->CreateGeometryShader(
				blob->GetBufferPointer(),
				blob->GetBufferSize(),
				nullptr,
				&gs);
		r_assert(hr);
	}
}

GShader::~GShader()
{
	if(gs)
		gs->Release();
}

void GShader::Apply(ID3D11DeviceContext* dContext)const
{
	if (gs)
	{
		dContext->GSSetShader(gs, nullptr, 0);

		for (auto i = cbs.begin(); i != cbs.end(); ++i)
		{
			dContext->GSSetConstantBuffers(i->first, i->second.arrayNum, i->second.data->GetAddress());
		}
		for (auto i = srvs.begin(); i != srvs.end(); ++i)
		{
			UINT slot = i->first;
			UINT arrayNum = i->second.arrayNum;
			ID3D11ShaderResourceView* srv = i->second.data;

			dContext->GSSetShaderResources(slot, arrayNum, &srv);
		}
		for (auto i = samps.begin(); i != samps.end(); ++i)
		{
			UINT slot = i->first;
			UINT arrayNum = i->second.arrayNum;
			ID3D11SamplerState* samp = i->second.data;

			dContext->GSSetSamplers(slot, arrayNum, &samp);
		}
	}
	else
	{
		dContext->GSSetShader(nullptr, nullptr, 0);
	}
}

PShader::PShader(ID3D11Device* device, std::string fileName)
{
	if (fileName != "")
	{
		std::wstring wPS(fileName.begin(), fileName.end());
		ID3DBlob* psBlob;
		HRESULT hr = D3DReadFileToBlob(
			(ShaderPath() + wPS).c_str(),
			&psBlob);
		r_assert(hr);
		hr = device->CreatePixelShader(
				psBlob->GetBufferPointer(),
				psBlob->GetBufferSize(),
				nullptr,
				&ps);
		r_assert(hr);
		psBlob->Release();
	}
}
PShader::~PShader()
{
	if(ps)
		ps->Release();
}
void PShader::Apply(ID3D11DeviceContext* dContext)const
{
	dContext->PSSetShader(ps, nullptr, 0);

	for (auto i = cbs.begin(); i != cbs.end(); ++i)
	{
		dContext->PSSetConstantBuffers(i->first, i->second.arrayNum, i->second.data->GetAddress());
	}
	for (auto i = srvs.begin(); i != srvs.end(); ++i)
	{
		UINT slot = i->first;
		UINT arrayNum = i->second.arrayNum;
		ID3D11ShaderResourceView* srv = i->second.data;

		dContext->PSSetShaderResources(slot, arrayNum, &srv);
	}
	for (auto i = samps.begin(); i != samps.end(); ++i)
	{
		UINT slot = i->first;
		UINT arrayNum = i->second.arrayNum;
		ID3D11SamplerState* samp = i->second.data;

		dContext->PSSetSamplers(slot, arrayNum, &samp);
	}
}

CShader::CShader(ID3D11Device* device, const std::string CSfileName)
{
	std::wstring wCS(CSfileName.begin(), CSfileName.end());
	ID3DBlob* csBlob;

	HRESULT hr=	D3DReadFileToBlob(
		(ShaderPath() + wCS).c_str(),
			&csBlob);
	r_assert(hr);
	hr = device->CreateComputeShader(
			csBlob->GetBufferPointer(),
			csBlob->GetBufferSize(),
			nullptr,
			&cs);
	r_assert(hr);
	csBlob->Release();
}

CShader::~CShader()
{
	if(cs)
		cs->Release();
}

void CShader::Apply(ID3D11DeviceContext* dContext)const
{
	dContext->CSSetShader(cs, nullptr, 0);

	for (auto i = cbs.begin(); i != cbs.end(); ++i)
	{
		dContext->CSSetConstantBuffers(i->first, i->second.arrayNum, i->second.data->GetAddress());
	}
	for (auto i = srvs.begin(); i != srvs.end(); ++i)
	{
		UINT slot = i->first;
		UINT arrayNum = i->second.arrayNum;
		ID3D11ShaderResourceView* srv = i->second.data;

		dContext->CSSetShaderResources(slot, arrayNum, &srv);
	}
	for (auto i = samps.begin(); i != samps.end(); ++i)
	{
		UINT slot = i->first;
		UINT arrayNum = i->second.arrayNum;
		ID3D11SamplerState* samp = i->second.data;

		dContext->CSSetSamplers(slot, arrayNum, &samp);
	}
}

HShader::HShader(ID3D11Device* device, std::string fileName)
{
	if (fileName == "")
		return;

	std::wstring wCS(fileName.begin(), fileName.end());
	ID3DBlob* blob;

	HRESULT hr =D3DReadFileToBlob(
		(ShaderPath() + wCS).c_str(),
			&blob);
	r_assert(hr);

	hr=device->CreateHullShader(
			blob->GetBufferPointer(),
			blob->GetBufferSize(),
			nullptr,
			&hs);
	r_assert(hr);
}

HShader::~HShader()
{
	if(hs)
		hs->Release();
}

void HShader::Apply(ID3D11DeviceContext* dContext)const
{
	dContext->HSSetShader(hs, nullptr, 0);

	for (auto i = cbs.begin(); i != cbs.end(); ++i)
	{
		dContext->HSSetConstantBuffers(i->first, i->second.arrayNum, i->second.data->GetAddress());
	}
	for (auto i = srvs.begin(); i != srvs.end(); ++i)
	{
		UINT slot = i->first;
		UINT arrayNum = i->second.arrayNum;
		ID3D11ShaderResourceView* srv = i->second.data;

		dContext->HSSetShaderResources(slot, arrayNum, &srv);
	}
	for (auto i = samps.begin(); i != samps.end(); ++i)
	{
		UINT slot = i->first;
		UINT arrayNum = i->second.arrayNum;
		ID3D11SamplerState* samp = i->second.data;

		dContext->HSSetSamplers(slot, arrayNum, &samp);
	}
}

DShader::DShader(ID3D11Device* device, std::string fileName)
{
	if (fileName == "")
		return;

	std::wstring wCS(fileName.begin(), fileName.end());
	ID3DBlob* blob;

	HRESULT hr = D3DReadFileToBlob(
		(ShaderPath() + wCS).c_str(),
			&blob);
	r_assert(hr);

	hr = device->CreateDomainShader(
			blob->GetBufferPointer(),
			blob->GetBufferSize(),
			nullptr,
			&ds);
	r_assert(hr);
	blob->Release();
}

DShader::~DShader()
{
	if(ds)
		ds->Release();
}

void DShader::Apply(ID3D11DeviceContext* dContext)const
{
	dContext->DSSetShader(ds, nullptr, 0);

	for (auto i = cbs.begin(); i != cbs.end(); ++i)
	{
		dContext->DSSetConstantBuffers(i->first, i->second.arrayNum, i->second.data->GetAddress());
	}
	for (auto i = srvs.begin(); i != srvs.end(); ++i)
	{
		UINT slot = i->first;
		UINT arrayNum = i->second.arrayNum;
		ID3D11ShaderResourceView* srv = i->second.data;

		dContext->DSSetShaderResources(slot, arrayNum, &srv);
	}
	for (auto i = samps.begin(); i != samps.end(); ++i)
	{
		UINT slot = i->first;
		UINT arrayNum = i->second.arrayNum;
		ID3D11SamplerState* samp = i->second.data;

		dContext->DSSetSamplers(slot, arrayNum, &samp);
	}
}
