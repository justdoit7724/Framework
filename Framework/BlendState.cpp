#include "BlendState.h"

BlendState::BlendState(ID3D11Device *device)
{
	D3D11_BLEND_DESC stateDesc;
	stateDesc.AlphaToCoverageEnable = false;
	stateDesc.IndependentBlendEnable = false;
	stateDesc.RenderTarget[0].BlendEnable = true;
	stateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	stateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	stateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	stateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	stateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	stateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	stateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	r_assert(
		device->CreateBlendState(
			&stateDesc,
			state.GetAddressOf())
	);
}

void BlendState::SetPipeline(ID3D11DeviceContext *dContext)
{
	float blendFactors[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	dContext->OMSetBlendState(state.Get(), blendFactors, 0xffffffff);
}

