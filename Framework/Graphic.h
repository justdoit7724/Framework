#pragma once

#include "DX_info.h"


class Graphic
{
public:
	Graphic(HWND _hwnd);

	void Present();

private:

	HWND hwnd;

	ComPtr<IDXGISwapChain> swapchain;
	ComPtr<ID3D11RenderTargetView> rtv;
	ComPtr<ID3D11DepthStencilView> dsView;
	ComPtr<ID3D11DepthStencilState> dsState;
};

