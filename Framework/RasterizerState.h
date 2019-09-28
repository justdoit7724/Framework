#pragma once
#include "Component.h"
class RasterizerState :
	public Component
{
public:
	RasterizerState(D3D11_RASTERIZER_DESC* desc);

	void Apply()const override;

private:
	ComPtr<ID3D11RasterizerState> state;
};

