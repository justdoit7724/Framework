#pragma once
#include "DefVertex.h"

class DepthStencilState;
class RasterizerState;
class BlendState;
class ResourceMgr;
class ConstantBuffer;

namespace DX
{
	class DXLIB_DLL Graphic
	{
	public:
		Graphic();
		~Graphic();

		int Initialize(HWND hwnd, int msaa);
		int Present()const;

		int SetPipelineData(PIP_REG id, const std::vector<const void*>& datas);

		const int m_id;

	private:
		Graphic(const Graphic&) = delete;
		Graphic& operator=(const Graphic&) = delete;


		HWND hwnd;

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

		ResourceMgr* m_ResourceMgr;
		std::unordered_map<int, ConstantBuffer*> m_mCB;
	};
}

