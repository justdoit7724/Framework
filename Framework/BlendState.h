#pragma once
#include "Component.h"

class BlendState : public Component
{
public:
	BlendState(ID3D11Device* device, D3D11_BLEND_DESC * desc = nullptr);
	~BlendState();

	void Modify(ID3D11Device* device, D3D11_BLEND_DESC* desc);
	void Apply(ID3D11DeviceContext* dContext);


private:
	ID3D11BlendState* state;
};

