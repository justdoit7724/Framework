#pragma once

#include "DX_info.h"
#include "Network.h"

class Graphic : public IGraphic
{
public:
	Graphic(HWND _hwnd);
	~Graphic();

	void Present();

	ID3D11Texture2D* BackBuffer()override {return backBuffer;};
	ID3D11Texture2D* DepthStencilBuffer()override {return depthStencilBuffer;}

private:

	HWND hwnd;

	ID3D11Texture2D* backBuffer;
	ID3D11Texture2D* depthStencilBuffer;
	ComPtr<IDXGISwapChain> swapchain;
	ComPtr<ID3D11RenderTargetView> rtv;
	ComPtr<ID3D11DepthStencilView> dsView;
	ComPtr<ID3D11RasterizerState> rasterizerState;
};

