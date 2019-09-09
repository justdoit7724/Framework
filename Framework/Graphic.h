#pragma once

#include "DX_info.h"
#include "Network.h"

class Graphic : public IGraphic
{
public:
	Graphic(HWND _hwnd);

	void Present();

	ID3D11Device* Device() {return device.Get();}
	ID3D11DeviceContext* DContext() {return dContext.Get();}

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

	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> dContext;
	ComPtr<IDXGISwapChain> swapchain;
	ComPtr<ID3D11RenderTargetView> rtv;
	ComPtr<ID3D11DepthStencilView> dsView;
	ComPtr<ID3D11RasterizerState> rasterizerState;
	ComPtr<ID3D11DepthStencilState> dsState;
};

