#pragma once
#include "Component.h"

class BlendState : public Component
{
public:
	BlendState(D3D11_BLEND_DESC * desc = nullptr);

	void Modify(D3D11_BLEND_DESC* desc);
	void Apply();


private:
	D3D11_BLEND_DESC desc;
	ComPtr<ID3D11BlendState> state;
};

