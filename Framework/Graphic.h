#pragma once

#include "DX_info.h"
#include "Network.h"

class Graphic : public IGraphic
{
public:
	Graphic(HWND _hwnd);

	void Present();

	void SetRasterizerState()
	{
		dContext->RSSetState(rasterizerState.Get());
	}
	void ClearDepth(float v)
	{
		dContext->ClearDepthStencilView(dsView.Get(), D3D11_CLEAR_DEPTH, v, 0);
	}

private:

	HWND hwnd;

	ComPtr<IDXGISwapChain> swapchain;
	ComPtr<ID3D11RenderTargetView> rtv;
	ComPtr<ID3D11DepthStencilView> dsView;
	ComPtr<ID3D11RasterizerState> rasterizerState;
	ComPtr<ID3D11DepthStencilState> dsState;
};

