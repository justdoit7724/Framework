#pragma once
#include "Component.h"

namespace DX
{
	class BlendState : public Component
	{
	public:
		BlendState(D3D11_BLEND_DESC* desc);
		~BlendState();

		void Modify(D3D11_BLEND_DESC* desc);
		void Apply()const override;


	private:
		D3D11_BLEND_DESC desc;
		ID3D11BlendState* state;
	};
}

