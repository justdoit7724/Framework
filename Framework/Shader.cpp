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

VPShader::VPShader(ID3D11Device* device, const std::wstring VSfileName, const std::wstring PSfileName, const D3D11_INPUT_ELEMENT_DESC * layoutDesc, int layoutNum)
{
	ComPtr<ID3DBlob> vsBlob;
	r_assert(
		D3DReadFileToBlob(
		(ShaderPath() + VSfileName).c_str(),
			vsBlob.GetAddressOf())
	);
	r_assert(
		device->CreateVertexShader(
			vsBlob->GetBufferPointer(),
			vsBlob->GetBufferSize(),
			nullptr,
			vs.GetAddressOf())
	);

	r_assert(
		device->CreateInputLayout(
			layoutDesc,
			layoutNum,
			vsBlob->GetBufferPointer(),
			vsBlob->GetBufferSize(),
			iLayout.GetAddressOf())
	);

	ComPtr<ID3DBlob> psBlob;
	r_assert(
		D3DReadFileToBlob(
		(ShaderPath() + PSfileName).c_str(),
			psBlob.GetAddressOf())
	);
	r_assert(
		device->CreatePixelShader(
			psBlob->GetBufferPointer(),
			psBlob->GetBufferSize(),
			nullptr,
			ps.GetAddressOf())
	);
}

void VPShader::Apply(ID3D11DeviceContext* dContext)
{
	dContext->IASetInputLayout(iLayout.Get());
	dContext->VSSetShader(vs.Get(), nullptr, 0);
	dContext->PSSetShader(ps.Get(), nullptr, 0);
}


CShader::CShader(ID3D11Device* device, const std::wstring CSfileName)
{
	ComPtr<ID3DBlob> csBlob;

	r_assert(
		D3DReadFileToBlob(
		(ShaderPath() + CSfileName).c_str(),
			csBlob.GetAddressOf())
	);
	r_assert(
		device->CreateComputeShader(
			csBlob->GetBufferPointer(),
			csBlob->GetBufferSize(),
			nullptr,
			cs.GetAddressOf())
	);
}

void CShader::Apply(ID3D11DeviceContext* dContext)
{
	dContext->CSSetShader(cs.Get(), nullptr, 0);
}
