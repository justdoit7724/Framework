#pragma once

namespace DX
{
	class DXLIB_DLL Graphic
	{
	public:
		Graphic(HWND _hwnd);
		~Graphic();

		void Present();
		ID3D11Device* Device();
		ID3D11DeviceContext* DContext();

	private:
		Graphic() = delete;

		HWND hwnd;

		ID3D11Device* m_device;
		ID3D11DeviceContext* m_dContext;

		ID3D11Texture2D* backBuffer;
		ID3D11Texture2D* depthStencilBuffer;
		D3D11_VIEWPORT viewport;
		IDXGISwapChain* swapchain;
		ID3D11RenderTargetView* rtv;
		ID3D11DepthStencilView* dsView;
		ID3D11RasterizerState* rasterizerState;
	};
}

