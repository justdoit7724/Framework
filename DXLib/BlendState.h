#pragma once

class BlendState
{
public:
	BlendState(ID3D11Device* device, D3D11_BLEND_DESC desc);
	~BlendState();

	void Modify(ID3D11Device* device, D3D11_BLEND_DESC desc);
	void Apply(ID3D11DeviceContext* dContext)const;


private:
	D3D11_BLEND_DESC desc;
	ID3D11BlendState* state;
};


