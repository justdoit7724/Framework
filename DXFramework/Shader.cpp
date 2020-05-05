
#include "Shader.h"
#include "Buffer.h"
#pragma comment(lib, "D3DCompiler.lib")


static const wchar_t* ShaderPath() {
#ifdef _DEBUG
#ifdef _WIN64
	return L"DXFramework\\HLSL\\_x64\\_Debug\\";
#else
	return L"DXFramework\\HLSL\\_Win32\\_Debug\\";
#endif
#else
#ifdef _WIN64
	return L"DXFramework\\HLSL\\_x64\\_Release\\";
#else
	return L"DXFramework\\HLSL\\_Win32\\_Release\\";
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

void Shader::AddCB(UINT slot, UINT arrayNum, UINT byteSize)
{
	if (cbs.find(slot) == cbs.end())
	{
		cbs.insert(std::pair<UINT, BindingCB>(slot, BindingCB(new Buffer(byteSize), arrayNum)));
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

void Shader::AddSamp(UINT slot, UINT arrayNum, D3D11_SAMPLER_DESC * desc)
{
	if (samps.find(slot) == samps.end())
	{
		ID3D11SamplerState* newSampler=nullptr;
		DX_Device->CreateSamplerState(desc, &newSampler);

		samps.insert(std::pair<UINT, BindingSamp>(slot, BindingSamp(newSampler, arrayNum)));
	}
	else
	{
		// slot overlap
		assert(false);
	}
}

void Shader::WriteCB(UINT slot, const void * data)
{
	if (cbs.find(slot)!=cbs.end() && data)
	{
		cbs[slot].data->Write(data);
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




VShader::VShader(std::string fileName, const D3D11_INPUT_ELEMENT_DESC * layoutDesc, UINT layoutNum)
{
	std::wstring wVS(fileName.begin(), fileName.end());
	ComPtr<ID3DBlob> vsBlob;
	HRESULT hr = D3DReadFileToBlob(
		(ShaderPath() + wVS).c_str(),
			vsBlob.GetAddressOf());
	r_assert(hr);
	hr = DX_Device->CreateVertexShader(
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		nullptr,
		vs.GetAddressOf());
	r_assert(hr);

	hr = DX_Device->CreateInputLayout(
			layoutDesc,
			layoutNum,
			vsBlob->GetBufferPointer(),
			vsBlob->GetBufferSize(),
			iLayout.GetAddressOf());
	r_assert(hr);

}


void VShader::Apply()const
{
	DX_DContext->IASetInputLayout(iLayout.Get());
	DX_DContext->VSSetShader(vs.Get(), nullptr, 0);

	for (auto i = cbs.begin(); i != cbs.end(); ++i)
	{
		DX_DContext->VSSetConstantBuffers(i->first, i->second.arrayNum, i->second.data->GetAddress());
	}
	for (auto i = srvs.begin(); i != srvs.end(); ++i)
	{
		UINT slot = i->first;
		UINT arrayNum = i->second.arrayNum;
		ID3D11ShaderResourceView* srv = i->second.data;

		DX_DContext->VSSetShaderResources(slot, arrayNum, &srv);
	}
	for (auto i = samps.begin(); i != samps.end(); ++i)
	{
		UINT slot = i->first;
		UINT arrayNum = i->second.arrayNum;
		ID3D11SamplerState* samp = i->second.data;

		DX_DContext->VSSetSamplers(slot, arrayNum, &samp);
	}
}


GShader::GShader(std::string fileName)
{
	if (fileName != "")
	{
		std::wstring wGS(fileName.begin(), fileName.end());
		ComPtr<ID3DBlob> blob;

		HRESULT hr = D3DReadFileToBlob(
			(ShaderPath() + wGS).c_str(),
				blob.GetAddressOf());
		r_assert(hr);
		hr = DX_Device->CreateGeometryShader(
				blob->GetBufferPointer(),
				blob->GetBufferSize(),
				nullptr,
				gs.GetAddressOf());
		r_assert(hr);
	}
}

void GShader::Apply()const
{
	if (gs)
	{
		DX_DContext->GSSetShader(gs.Get(), nullptr, 0);

		for (auto i = cbs.begin(); i != cbs.end(); ++i)
		{
			DX_DContext->GSSetConstantBuffers(i->first, i->second.arrayNum, i->second.data->GetAddress());
		}
		for (auto i = srvs.begin(); i != srvs.end(); ++i)
		{
			UINT slot = i->first;
			UINT arrayNum = i->second.arrayNum;
			ID3D11ShaderResourceView* srv = i->second.data;

			DX_DContext->GSSetShaderResources(slot, arrayNum, &srv);
		}
		for (auto i = samps.begin(); i != samps.end(); ++i)
		{
			UINT slot = i->first;
			UINT arrayNum = i->second.arrayNum;
			ID3D11SamplerState* samp = i->second.data;

			DX_DContext->GSSetSamplers(slot, arrayNum, &samp);
		}
	}
	else
	{
		DX_DContext->GSSetShader(nullptr, nullptr, 0);
	}
}

PShader::PShader(std::string fileName)
{
	if (fileName != "")
	{
		std::wstring wPS(fileName.begin(), fileName.end());
		ComPtr<ID3DBlob> psBlob;
		HRESULT hr = D3DReadFileToBlob(
			(ShaderPath() + wPS).c_str(),
			psBlob.GetAddressOf());
		r_assert(hr);
		hr = DX_Device->CreatePixelShader(
				psBlob->GetBufferPointer(),
				psBlob->GetBufferSize(),
				nullptr,
				ps.GetAddressOf());
		r_assert(hr);
	}
}
void PShader::Apply()const
{
	DX_DContext->PSSetShader(ps.Get(), nullptr, 0);

	for (auto i = cbs.begin(); i != cbs.end(); ++i)
	{
		DX_DContext->PSSetConstantBuffers(i->first, i->second.arrayNum, i->second.data->GetAddress());
	}
	for (auto i = srvs.begin(); i != srvs.end(); ++i)
	{
		UINT slot = i->first;
		UINT arrayNum = i->second.arrayNum;
		ID3D11ShaderResourceView* srv = i->second.data;

		DX_DContext->PSSetShaderResources(slot, arrayNum, &srv);
	}
	for (auto i = samps.begin(); i != samps.end(); ++i)
	{
		UINT slot = i->first;
		UINT arrayNum = i->second.arrayNum;
		ID3D11SamplerState* samp = i->second.data;

		DX_DContext->PSSetSamplers(slot, arrayNum, &samp);
	}
}

CShader::CShader(const std::string CSfileName)
{
	std::wstring wCS(CSfileName.begin(), CSfileName.end());
	ComPtr<ID3DBlob> csBlob;

	HRESULT hr=	D3DReadFileToBlob(
		(ShaderPath() + wCS).c_str(),
			csBlob.GetAddressOf());
	r_assert(hr);
	hr = DX_Device->CreateComputeShader(
			csBlob->GetBufferPointer(),
			csBlob->GetBufferSize(),
			nullptr,
			cs.GetAddressOf());
	r_assert(hr);
}

void CShader::Apply()const
{
	DX_DContext->CSSetShader(cs.Get(), nullptr, 0);

	for (auto i = cbs.begin(); i != cbs.end(); ++i)
	{
		DX_DContext->CSSetConstantBuffers(i->first, i->second.arrayNum, i->second.data->GetAddress());
	}
	for (auto i = srvs.begin(); i != srvs.end(); ++i)
	{
		UINT slot = i->first;
		UINT arrayNum = i->second.arrayNum;
		ID3D11ShaderResourceView* srv = i->second.data;

		DX_DContext->CSSetShaderResources(slot, arrayNum, &srv);
	}
	for (auto i = samps.begin(); i != samps.end(); ++i)
	{
		UINT slot = i->first;
		UINT arrayNum = i->second.arrayNum;
		ID3D11SamplerState* samp = i->second.data;

		DX_DContext->CSSetSamplers(slot, arrayNum, &samp);
	}
}

HShader::HShader(std::string fileName)
{
	if (fileName == "")
		return;

	std::wstring wCS(fileName.begin(), fileName.end());
	ComPtr<ID3DBlob> blob;

	HRESULT hr =D3DReadFileToBlob(
		(ShaderPath() + wCS).c_str(),
			blob.GetAddressOf());
	r_assert(hr);

	hr=DX_Device->CreateHullShader(
			blob->GetBufferPointer(),
			blob->GetBufferSize(),
			nullptr,
			hs.GetAddressOf());
	r_assert(hr);
}

void HShader::Apply()const
{
	DX_DContext->HSSetShader(hs.Get(), nullptr, 0);

	for (auto i = cbs.begin(); i != cbs.end(); ++i)
	{
		DX_DContext->HSSetConstantBuffers(i->first, i->second.arrayNum, i->second.data->GetAddress());
	}
	for (auto i = srvs.begin(); i != srvs.end(); ++i)
	{
		UINT slot = i->first;
		UINT arrayNum = i->second.arrayNum;
		ID3D11ShaderResourceView* srv = i->second.data;

		DX_DContext->HSSetShaderResources(slot, arrayNum, &srv);
	}
	for (auto i = samps.begin(); i != samps.end(); ++i)
	{
		UINT slot = i->first;
		UINT arrayNum = i->second.arrayNum;
		ID3D11SamplerState* samp = i->second.data;

		DX_DContext->HSSetSamplers(slot, arrayNum, &samp);
	}
}

DShader::DShader(std::string fileName)
{
	if (fileName == "")
		return;

	std::wstring wCS(fileName.begin(), fileName.end());
	ComPtr<ID3DBlob> blob;

	HRESULT hr = D3DReadFileToBlob(
		(ShaderPath() + wCS).c_str(),
			blob.GetAddressOf());
	r_assert(hr);

	hr = DX_Device->CreateDomainShader(
			blob->GetBufferPointer(),
			blob->GetBufferSize(),
			nullptr,
			ds.GetAddressOf());
	r_assert(hr);
}

void DShader::Apply()const
{
	DX_DContext->DSSetShader(ds.Get(), nullptr, 0);

	for (auto i = cbs.begin(); i != cbs.end(); ++i)
	{
		DX_DContext->DSSetConstantBuffers(i->first, i->second.arrayNum, i->second.data->GetAddress());
	}
	for (auto i = srvs.begin(); i != srvs.end(); ++i)
	{
		UINT slot = i->first;
		UINT arrayNum = i->second.arrayNum;
		ID3D11ShaderResourceView* srv = i->second.data;

		DX_DContext->DSSetShaderResources(slot, arrayNum, &srv);
	}
	for (auto i = samps.begin(); i != samps.end(); ++i)
	{
		UINT slot = i->first;
		UINT arrayNum = i->second.arrayNum;
		ID3D11SamplerState* samp = i->second.data;

		DX_DContext->DSSetSamplers(slot, arrayNum, &samp);
	}
}
