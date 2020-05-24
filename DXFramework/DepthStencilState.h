#pragma once

#include "Component.h"

class DepthStencilState :
	public Component
{
private:

	ID3D11DepthStencilState* state;

	UINT refValue;

public:
	DepthStencilState(D3D11_DEPTH_STENCIL_DESC* desc);
	~DepthStencilState();
	void Modify(D3D11_DEPTH_STENCIL_DESC* desc);
	void Apply() const;
	void SetRefValue(UINT v);
};

