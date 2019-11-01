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
	ID3D11RenderTargetView* RenderTargetView() override{return rtv.Get();}
	ID3D11Texture2D* DepthStencilBuffer()override {return depthStencilBuffer;}
	ID3D11DepthStencilView* DepthStencilView() override{return dsView.Get();}
	void RestoreViewport()override {DX_DContext->RSSetViewports(1, &viewport);}
	void RestoreRTV()override { 

		DX_DContext->OMSetRenderTargets(1, rtv.GetAddressOf(), dsView.Get()); 
	}

private:

	HWND hwnd;

	ID3D11Texture2D* backBuffer;
	ID3D11Texture2D* depthStencilBuffer;
	D3D11_VIEWPORT viewport;
	ComPtr<IDXGISwapChain> swapchain;
	ComPtr<ID3D11RenderTargetView> rtv;
	ComPtr<ID3D11DepthStencilView> dsView;
	ComPtr<ID3D11RasterizerState> rasterizerState;
};

