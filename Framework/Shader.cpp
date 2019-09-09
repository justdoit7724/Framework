#include "Shader.h"

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

VPShader::VPShader(std::string VSfileName, std::string PSfileName, const D3D11_INPUT_ELEMENT_DESC * layoutDesc, int layoutNum)
{
	std::wstring wVS(VSfileName.begin(), VSfileName.end());
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

	std::wstring wPS(PSfileName.begin(), PSfileName.end());
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

void VPShader::Apply()
{
	DX_DContext->IASetInputLayout(iLayout.Get());
	DX_DContext->VSSetShader(vs.Get(), nullptr, 0);
	DX_DContext->PSSetShader(ps.Get(), nullptr, 0);
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
}
