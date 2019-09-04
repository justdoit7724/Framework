#pragma once
#include "DX_info.h"

class BlendState
{
public:
	BlendState(ID3D11Device*);

	void SetPipeline(ID3D11DeviceContext*);

private:
	ComPtr<ID3D11BlendState> state;
};

