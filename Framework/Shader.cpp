#include "Shader.h"
#include "Buffer.h"

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
	for (auto srv : srvs)
	{
		srv.second.data->Release();
	}
	for (auto samp : samps)
	{
		delete samp.second.data;
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

void Shader::AddSRV(UINT slot, UINT arrayNum, ID3D11ShaderResourceView * srv)
{
	if (srvs.find(slot) == srvs.end())
	{
		srvs.insert(std::pair<UINT, BindingSRV>(slot, BindingSRV(srv, arrayNum)));
	}
	else
	{
		// slot overlap
		assert(false);
	}
	srvs[slot].arrayNum = arrayNum;
	srvs[slot].data = srv;
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

void Shader::WriteCB(UINT slot, void * data)
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




VShader::VShader(std::string fileName, const D3D11_INPUT_ELEMENT_DESC * layoutDesc, int layoutNum)
{
	std::wstring wVS(fileName.begin(), fileName.end());
	ComPtr<ID3DBlob> vsBlob;
	r_assert(
		D3DReadFileToBlob(
		(ShaderPath() + wVS).c_str(),
			vsBlob.GetAddressOf())
	);
	r_assert(
		DX_Device->CreateVertexShader(
			vsBlob->GetBufferPointer(),
			vsBlob->GetBufferSize(),
			nullptr,
			vs.GetAddressOf())
	);

	r_assert(
		DX_Device->CreateInputLayout(
			layoutDesc,
			layoutNum,
			vsBlob->GetBufferPointer(),
			vsBlob->GetBufferSize(),
			iLayout.GetAddressOf())
	);
}

PShader::PShader(std::string fileName)
{
	std::wstring wPS(fileName.begin(), fileName.end());
	ComPtr<ID3DBlob> psBlob;
	r_assert(
		D3DReadFileToBlob(
		(ShaderPath() + wPS).c_str(),
			psBlob.GetAddressOf())
	);
	r_assert(
		DX_Device->CreatePixelShader(
			psBlob->GetBufferPointer(),
			psBlob->GetBufferSize(),
			nullptr,
			ps.GetAddressOf())
	);
}

void VShader::Apply()
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
	:enabled(fileName != "")
{
	if (!enabled)
		return;

	std::wstring wGS(fileName.begin(), fileName.end());
	ComPtr<ID3DBlob> blob;

	r_assert(
		D3DReadFileToBlob(
		(ShaderPath() + wGS).c_str(),
			blob.GetAddressOf())
	);
	r_assert(
		DX_Device->CreateGeometryShader(
			blob->GetBufferPointer(),
			blob->GetBufferSize(),
			nullptr,
			gs.GetAddressOf())
	);
}

void GShader::Apply()
{
	if (enabled)
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

void PShader::Apply()
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

	r_assert(
		D3DReadFileToBlob(
		(ShaderPath() + wCS).c_str(),
			csBlob.GetAddressOf())
	);
	r_assert(
		DX_Device->CreateComputeShader(
			csBlob->GetBufferPointer(),
			csBlob->GetBufferSize(),
			nullptr,
			cs.GetAddressOf())
	);
}

void CShader::Apply()
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

