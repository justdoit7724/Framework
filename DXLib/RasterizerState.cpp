
#include "pch.h"

#include "RasterizerState.h"

RasterizerState::RasterizerState(ID3D11Device* device, D3D11_RASTERIZER_DESC* desc)
{
	D3D11_RASTERIZER_DESC curDesc;

	if (desc == nullptr)
	{
		ZeroMemory(&curDesc, sizeof(D3D11_RASTERIZER_DESC));
		curDesc.FillMode = D3D11_FILL_SOLID;
		curDesc.CullMode = D3D11_CULL_BACK;
		curDesc.FrontCounterClockwise = false;
	}
	else
	{
		curDesc = *desc;
	}

	HRESULT hr = device->CreateRasterizerState(&curDesc, &state);
	if (FAILED(hr))
		return;
}
RasterizerState::~RasterizerState()
{
	state->Release();
}
void RasterizerState::Apply(ID3D11DeviceContext* dContext)const
{
	dContext->RSSetState(state);
}
