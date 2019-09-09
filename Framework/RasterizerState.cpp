#include "RasterizerState.h"

RasterizerState::RasterizerState(D3D11_RASTERIZER_DESC * desc)
{
	ZeroMemory(&(this->desc), sizeof(D3D11_RASTERIZER_DESC));
	if (desc)
	{
		this->desc = *desc;
	}
	else
	{
		this->desc.CullMode = D3D11_CULL_BACK;
		this->desc.FillMode = D3D11_FILL_SOLID;
	}

	r_assert(
		DX_Device->CreateRasterizerState(&(this->desc), state.GetAddressOf())
	);
}


void RasterizerState::Apply()
{
	DX_DContext->RSSetState(state.Get());
}
