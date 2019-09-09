#include "DepthStencilState.h"



DepthStencilState::DepthStencilState(D3D11_DEPTH_STENCIL_DESC* desc)
	:refValue(0)
{
	D3D11_DEPTH_STENCIL_DESC firstDesc;
	if (desc)
	{
		firstDesc = *desc;
	}
	else
	{
		firstDesc.DepthEnable = true;
		firstDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		firstDesc.DepthFunc = D3D11_COMPARISON_LESS;
		firstDesc.StencilEnable = false;
		/*firstDesc.StencilReadMask = 0xff;
		firstDesc.StencilWriteMask = 0xff;
		D3D11_DEPTH_STENCILOP_DESC oDesc;
		oDesc.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		oDesc.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		oDesc.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		oDesc.StencilFunc = D3D11_COMPARISON_EQUAL;
		firstDesc.FrontFace= oDesc;
		firstDesc.BackFace= oDesc;*/
	}

	r_assert(
		DX_Device->CreateDepthStencilState(&firstDesc, &state)
	);
}

void DepthStencilState::Modify(D3D11_DEPTH_STENCIL_DESC * desc)
{
	state->Release();

	r_assert(
		DX_Device->CreateDepthStencilState(desc, &state)
	);
}

void DepthStencilState::Apply()
{
	DX_DContext->OMSetDepthStencilState(state, refValue);
}

void DepthStencilState::SetRefValue(UINT v)
{
	refValue = v;
}


DepthStencilState::~DepthStencilState()
{
}
