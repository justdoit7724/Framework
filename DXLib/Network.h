#pragma once
#include <d3d11.h>
#include <vector>

namespace DX {
	class DXLIB_DLL IGraphic
	{
	public:
		virtual ID3D11Texture2D* BackBuffer() = 0;
		virtual ID3D11RenderTargetView* RenderTargetView() = 0;
		virtual ID3D11Texture2D* DepthStencilBuffer() = 0;
		virtual ID3D11DepthStencilView* DepthStencilView() = 0;
		virtual void RestoreViewport() = 0;
		virtual void RestoreRTV() = 0;
	};

	class DXLIB_DLL IDebug
	{
	public:
		virtual void Visualize() = 0;
	};
}