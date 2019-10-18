#pragma once
#include <d3d11.h>

class IGraphic
{
public:
	virtual ID3D11Texture2D* BackBuffer() = 0;
	virtual ID3D11Texture2D* DepthStencilBuffer() = 0;
	virtual void RestoreViewport() = 0;
	virtual void RestoreRTV() = 0;
};
