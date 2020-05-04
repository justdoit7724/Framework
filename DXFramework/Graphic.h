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
	ComPtr<IDXGISwapChain> swapchain;
	ComPtr<ID3D11RenderTargetView> rtv;
	ComPtr<ID3D11DepthStencilView> dsView;
	ComPtr<ID3D11RasterizerState> rasterizerState;
};

