#pragma once
#include "DefVertex.h"

class DepthStencilState;
class RasterizerState;
class BlendState;
class ResourceMgr;
class ConstantBuffer;

namespace DX
{
	class VertexLayout;

	class DXLIB_DLL Graphic
	{
	public:
		Graphic();
		~Graphic();

<<<<<<< HEAD
		int Initialize(HWND hwnd, int msaa);
		int Present()const;

		int SetPipelineData(PIP_REG id, const std::vector<const void*>& datas);

		const int m_id;
=======
		void Present();
		void BindView();
		ID3D11Device* Device();
		ID3D11DeviceContext* DContext();
		ID3D11Texture2D* DepthBuffer();
		ID3D11Texture2D* BackBuffer();
		ID3D11DepthStencilView* DSV();
		ID3D11RenderTargetView* RTV();
		HWND GetHWND();
>>>>>>> 03_DepthPeeling

	private:
		Graphic(const Graphic&) = delete;
		Graphic& operator=(const Graphic&) = delete;


		HWND m_hwnd;

		void* m_device;
		void* m_dContext;

		void* m_swapchain;
		void* m_backBuffer;
		void* m_depthStencilBuffer;
		
		void* m_viewport;
		DepthStencilState* m_dsDepthEnabled;
		DepthStencilState* m_dsDepthDisabled;
		RasterizerState* m_rsStatge;
		BlendState* m_blendOpaque;
		BlendState* m_blendTransparent;

		void* m_rtv;
		void* m_dsView;

<<<<<<< HEAD
		ResourceMgr* m_ResourceMgr;
		std::unordered_map<int, ConstantBuffer*> m_mCB;
=======
		ID3D11Texture2D* m_backBuffer;
		ID3D11Texture2D* m_depthStencilBuffer;
		D3D11_VIEWPORT m_viewport;
		IDXGISwapChain* m_swapchain;
		ID3D11RenderTargetView* m_rtv;
		ID3D11DepthStencilView* m_dsView;
		ID3D11RasterizerState* m_rasterizerState;


>>>>>>> 03_DepthPeeling
	};


	const DXLIB_DLL VertexLayout& D3DLayout_Simple();
	const DXLIB_DLL VertexLayout& D3DLayout_Std();

}

