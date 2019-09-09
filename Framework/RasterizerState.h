#pragma once
#include "Component.h"
class RasterizerState :
	public Component
{
public:
	RasterizerState(D3D11_RASTERIZER_DESC* desc = nullptr);

	void Apply();

private:

	D3D11_RASTERIZER_DESC desc;
	ComPtr<ID3D11RasterizerState> state;


};

