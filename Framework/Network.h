#pragma once
#include <d3d11.h>

class IGraphic
{
public:
	virtual void SetRasterizerState() = 0;
	virtual void ClearDepth(float v=1) = 0;

	virtual ID3D11Device* Device() = 0;
	virtual ID3D11DeviceContext* DContext() = 0;
};