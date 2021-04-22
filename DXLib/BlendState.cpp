
#include "pch.h"
#include "BlendState.h"

BlendState::BlendState(ID3D11Device* device, D3D11_BLEND_DESC desc)
	:desc(desc)
{
	HRESULT hr = device->CreateBlendState(
		&desc,
		&state);
	assert(SUCCEEDED(hr));
}

BlendState::~BlendState()
{
	state->Release();
}

void BlendState::Modify(ID3D11Device* device, D3D11_BLEND_DESC desc)
{
	state->Release();

	HRESULT hr = device->CreateBlendState(&desc, &state);
	assert(SUCCEEDED(hr));
}

void BlendState::Apply(ID3D11DeviceContext* dContext) const
{
	float blendFactors[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	dContext->OMSetBlendState(state, blendFactors, 0xffffffff);
}

