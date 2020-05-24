
#include "BlendState.h"

BlendState::BlendState(D3D11_BLEND_DESC * desc)
{
	if (desc)
	{
		this->desc = *desc;
	}
	else {
		this->desc.AlphaToCoverageEnable = false;
		this->desc.IndependentBlendEnable = false;
		this->desc.RenderTarget[0].BlendEnable = true;
		this->desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		this->desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		this->desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
		this->desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		this->desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		this->desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		this->desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}

	HRESULT hr = DX_Device->CreateBlendState(
		&(this->desc),
		&state);
	r_assert(hr);
}

BlendState::~BlendState()
{
	state->Release();
}

void BlendState::Modify(D3D11_BLEND_DESC * desc)
{
	state->Release();

	HRESULT hr = DX_Device->CreateBlendState(desc, &state);
	r_assert(hr);
}

void BlendState::Apply() const
{
	float blendFactors[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	DX_DContext->OMSetBlendState(state, blendFactors, 0xffffffff);
}

