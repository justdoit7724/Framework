#pragma once
#include "Component.h"

class BlendState : public Component
{
public:
	BlendState(D3D11_BLEND_DESC * desc);

	void Modify(D3D11_BLEND_DESC* desc);
	void Apply()const override ;


private:
	D3D11_BLEND_DESC desc;
	ComPtr<ID3D11BlendState> state;
};

