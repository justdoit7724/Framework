
#include "pch.h"

#include "DepthStencilState.h"

DepthStencilState::DepthStencilState(ID3D11Device* device, D3D11_DEPTH_STENCIL_DESC desc)
	:refValue(0)
{

	HRESULT hr = device->CreateDepthStencilState(&desc, &state);
	assert(SUCCEEDED( hr));
}

void DepthStencilState::Modify(ID3D11Device* device, D3D11_DEPTH_STENCIL_DESC desc)
{
	state->Release();
	HRESULT hr = device->CreateDepthStencilState(&desc, &state);
	assert(SUCCEEDED(hr));
}

void DepthStencilState::Apply(ID3D11DeviceContext* dContext) const
{
	dContext->OMSetDepthStencilState(state, refValue);
}

void DepthStencilState::SetRefValue(UINT v)
{
	refValue = v;
}


DepthStencilState::~DepthStencilState()
{
	state->Release();
}
