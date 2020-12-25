#pragma once

namespace DX
{
	class DXLIB_DLL Graphic
	{
	public:
		Graphic(HWND _hwnd, int msaa=1);
		~Graphic();

		void Present();
		void BindView();
		ID3D11Device* Device();
		ID3D11DeviceContext* DContext();
		ID3D11Texture2D* DepthBuffer();
		ID3D11Texture2D* BackBuffer();
		ID3D11DepthStencilView* DSV();
		ID3D11RenderTargetView* RTV();

	private:
		Graphic() = delete;

		HWND hwnd;

		ID3D11Device* m_device;
		ID3D11DeviceContext* m_dContext;

		ID3D11Texture2D* m_backBuffer;
		ID3D11Texture2D* m_depthStencilBuffer;
		D3D11_VIEWPORT m_viewport;
		IDXGISwapChain* m_swapchain;
		ID3D11RenderTargetView* m_rtv;
		ID3D11DepthStencilView* m_dsView;
		ID3D11RasterizerState* m_rasterizerState;
	};
}

