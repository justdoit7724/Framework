#pragma once

#include "Component.h"
class RasterizerState :
	public Component
{
public:
	RasterizerState(D3D11_RASTERIZER_DESC* desc);
	~RasterizerState();

	void Apply()const override;

private:
	ID3D11RasterizerState* state;
};

