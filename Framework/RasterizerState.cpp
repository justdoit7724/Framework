#include "RasterizerState.h"

RasterizerState::RasterizerState(D3D11_RASTERIZER_DESC* desc)
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

	r_assert(
		DX_Device->CreateRasterizerState(&curDesc, state.GetAddressOf())
	);
}
void RasterizerState::Apply()const
{
	DX_DContext->RSSetState(state.Get());
}
