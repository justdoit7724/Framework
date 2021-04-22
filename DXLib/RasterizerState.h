#pragma once

class RasterizerState
{
public:
	RasterizerState(ID3D11Device* device, D3D11_RASTERIZER_DESC* desc);
	~RasterizerState();

	void Apply(ID3D11DeviceContext* dContext)const ;

private:
	ID3D11RasterizerState* state;
};
