#pragma once
#include "Component.h"

class DepthStencilState :
	public Component
{
private:

	ID3D11DepthStencilState* state;

	UINT refValue;

public:
	DepthStencilState(ID3D11Device* device, D3D11_DEPTH_STENCIL_DESC* desc=nullptr);
	~DepthStencilState();
	void Modify(ID3D11Device* device, D3D11_DEPTH_STENCIL_DESC* desc);
	void Apply(ID3D11DeviceContext* dContext);
	void SetRefValue(UINT v);
};

