#pragma once
#include "Component.h"

class BlendState : public Component
{
public:
	BlendState(D3D11_BLEND_DESC * desc = nullptr);
	~BlendState();

	void Modify(D3D11_BLEND_DESC* desc);
	void Apply();


private:
	ID3D11BlendState* state;
};

