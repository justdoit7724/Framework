
#include "SetupScene.h"
#include "ShaderReg.h"

void SetupSamp(int reg, D3D11_FILTER filter)
{
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(D3D11_SAMPLER_DESC));
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	FLOAT black[4] = { 0,0,0,1 };
	sampDesc.BorderColor[0] = 0;
	sampDesc.BorderColor[1] = 0;
	sampDesc.BorderColor[2] = 0;
	sampDesc.BorderColor[3] = 1;
	sampDesc.Filter = filter;
	ID3D11SamplerState* samp;
	HRESULT hr = DX_Device->CreateSamplerState(&sampDesc, &samp);
	r_assert(hr);
	DX_DContext->PSSetSamplers(reg, 1, &samp);
	samp->Release();
}

SetupScene::SetupScene()
{
	SetupSamp(SHADER_REG_SAMP_POINT, D3D11_FILTER_MIN_MAG_MIP_POINT);
}

void SetupScene::Update(float elapsed, float spf)
{
}
