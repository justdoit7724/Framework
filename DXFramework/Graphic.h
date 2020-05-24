#pragma once

#include "DX_info.h"

class Graphic
{
public:
	Graphic(HWND _hwnd, float resX, float resY);
	~Graphic();

	void Present();

private:
	Graphic() {}

	HWND hwnd;

	ID3D11Texture2D* backBuffer;
	ID3D11Texture2D* depthStencilBuffer;
	D3D11_VIEWPORT viewport;
	IDXGISwapChain* swapchain;
	ID3D11RenderTargetView* rtv;
	ID3D11DepthStencilView* dsView;
	ID3D11RasterizerState* rasterizerState;
};

