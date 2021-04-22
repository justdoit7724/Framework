

#include "pch.h"

#include "Graphic.h"
#include "ShaderReg.h"
#include "Buffer.h"
#include "ResourceMgr.h"
#include "Shader.h"
#include "DepthStencilState.h"
#include "RasterizerState.h"
#include "BlendState.h"

namespace DX {

	int g_id = 0;

	int Graphic::Initialize(HWND _hwnd, int msaa)
	{
		assert(msaa == 1 || msaa == 2 || msaa == 4 || msaa == 8 || msaa == 16);

		RECT rc;
		if (!GetClientRect(_hwnd, &rc))
		{
			return 1;
		}
		int iWidth = rc.right - rc.left;
		int iHeight = rc.bottom - rc.top;

		hwnd = _hwnd;

		DXGI_SWAP_CHAIN_DESC scd;
		ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

		scd.BufferDesc.Width = (UINT)iWidth;
		scd.BufferDesc.Height = (UINT)iHeight;
		scd.BufferDesc.RefreshRate.Numerator = 60;
		scd.BufferDesc.RefreshRate.Denominator = 1;
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		scd.SampleDesc.Count = msaa;
		scd.SampleDesc.Quality = 0;

		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.BufferCount = 1;
		scd.OutputWindow = hwnd;
		scd.Windowed = TRUE;
		scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		HRESULT hr = D3D11CreateDeviceAndSwapChain(
			NULL,
			D3D_DRIVER_TYPE_HARDWARE,
			NULL,
			D3D11_CREATE_DEVICE_DEBUG,
			NULL,
			0,
			D3D11_SDK_VERSION,
			&scd,
			(IDXGISwapChain**)&m_swapchain,
			(ID3D11Device**)&m_device,
			NULL,
			(ID3D11DeviceContext**)&m_dContext);
		if (FAILED(hr))
			return 1;

		hr = ((IDXGISwapChain*)m_swapchain)->GetBuffer(
			0,
			__uuidof(ID3D11Texture2D),
			reinterpret_cast<void**>(&m_backBuffer));
		if (FAILED(hr))
			return 1;

		hr = ((ID3D11Device*)m_device)->CreateRenderTargetView(
			(ID3D11Texture2D*)m_backBuffer,
			nullptr,
			(ID3D11RenderTargetView **)&m_rtv);
		if (FAILED(hr))
			return 1;

		// register point sampler
		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(D3D11_SAMPLER_DESC));
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.BorderColor[0] = 0;
		sampDesc.BorderColor[1] = 0;
		sampDesc.BorderColor[2] = 0;
		sampDesc.BorderColor[3] = 1;
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		ID3D11SamplerState* sampTemp;
		hr = ((ID3D11Device*)m_device)->CreateSamplerState(&sampDesc, &sampTemp);
		if (FAILED(hr))
			return 1;
		((ID3D11DeviceContext*)m_dContext)->PSSetSamplers(SHADER_REG_SAMP_POINT, 1, &sampTemp);
		sampTemp->Release();

		ZeroMemory(&sampDesc, sizeof(D3D11_SAMPLER_DESC));
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.BorderColor[0] = 0;
		sampDesc.BorderColor[1] = 0;
		sampDesc.BorderColor[2] = 0;
		sampDesc.BorderColor[3] = 1;
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		hr = ((ID3D11Device*)m_device)->CreateSamplerState(&sampDesc, &sampTemp);
		if (FAILED(hr))
			return 1;
		((ID3D11DeviceContext*)m_dContext)->PSSetSamplers(SHADER_REG_SAMP_LINEAR, 1, &sampTemp);
		sampTemp->Release();


#pragma region Depth&Stencil Buffer

		D3D11_TEXTURE2D_DESC ds_desc;
		ds_desc.Width = scd.BufferDesc.Width;
		ds_desc.Height = scd.BufferDesc.Height;
		ds_desc.MipLevels = 1;
		ds_desc.ArraySize = 1;
		// TYPELESS instead of UNORM for copying data
		ds_desc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		ds_desc.SampleDesc = scd.SampleDesc;
		ds_desc.Usage = D3D11_USAGE_DEFAULT;
		ds_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		ds_desc.CPUAccessFlags = 0;
		ds_desc.MiscFlags = 0;

		hr = ((ID3D11Device*)m_device)->CreateTexture2D(
			&ds_desc,
			nullptr,
			(ID3D11Texture2D**)&m_depthStencilBuffer);
		if (FAILED(hr))
			return 1;

		D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc;
		ZeroMemory(&dsv_desc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		dsv_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsv_desc.Flags = 0;
		if (msaa == 1)
		{
			dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			dsv_desc.Texture2D.MipSlice = 0;
		}
		else
		{
			dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
		}
		hr = ((ID3D11Device*)m_device)->CreateDepthStencilView(
			(ID3D11Texture2D*)m_depthStencilBuffer,
			&dsv_desc,
			(ID3D11DepthStencilView **)&m_dsView);
		if (FAILED(hr))
			return 1;
		((ID3D11DeviceContext*)m_dContext)->OMSetRenderTargets(1, (ID3D11RenderTargetView**)&m_rtv, (ID3D11DepthStencilView * )m_dsView);

#pragma endregion

		m_viewport = new D3D11_VIEWPORT;
		ZeroMemory(m_viewport, sizeof(D3D11_VIEWPORT));
		((D3D11_VIEWPORT*)m_viewport)->TopLeftX = 0;
		((D3D11_VIEWPORT*)m_viewport)->TopLeftY = 0;
		((D3D11_VIEWPORT*)m_viewport)->Width = scd.BufferDesc.Width;
		((D3D11_VIEWPORT*)m_viewport)->Height = scd.BufferDesc.Height;
		((D3D11_VIEWPORT*)m_viewport)->MinDepth = 0.0f;
		((D3D11_VIEWPORT*)m_viewport)->MaxDepth = 1.0f;

		//rasterizer state
		D3D11_RASTERIZER_DESC rs_desc;
		ZeroMemory(&rs_desc, sizeof(D3D11_RASTERIZER_DESC));
		rs_desc.FillMode = D3D11_FILL_SOLID;
		rs_desc.CullMode = D3D11_CULL_BACK;
		ID3D11RasterizerState* rasterizerState;
		hr = ((ID3D11Device*)m_device)->CreateRasterizerState(&rs_desc, &rasterizerState);
		if (FAILED(hr))
			return 1;
		((ID3D11DeviceContext*)m_dContext)->RSSetState(rasterizerState);
		rasterizerState->Release();

		//depth stencil state
		D3D11_DEPTH_STENCIL_DESC dsDepthEnabledDesc;
		ZeroMemory(&dsDepthEnabledDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
		dsDepthEnabledDesc.DepthEnable = TRUE;
		dsDepthEnabledDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		dsDepthEnabledDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDepthEnabledDesc.StencilEnable = FALSE;
		m_dsDepthEnabled = new DepthStencilState((ID3D11Device*)m_device, dsDepthEnabledDesc);
		D3D11_DEPTH_STENCIL_DESC dsDepthDisabledDesc;
		ZeroMemory(&dsDepthDisabledDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
		dsDepthDisabledDesc.DepthEnable = FALSE;
		dsDepthDisabledDesc.StencilEnable = FALSE;
		m_dsDepthDisabled = new DepthStencilState((ID3D11Device*)m_device, dsDepthDisabledDesc);

		//blending state
		D3D11_BLEND_DESC blendOpaqueDesc;
		ZeroMemory(&blendOpaqueDesc, sizeof(D3D11_BLEND_DESC));
		blendOpaqueDesc.AlphaToCoverageEnable = FALSE;
		blendOpaqueDesc.IndependentBlendEnable = FALSE;
		blendOpaqueDesc.RenderTarget[0].BlendEnable = FALSE;
		m_blendOpaque = new BlendState((ID3D11Device*)m_device, blendOpaqueDesc);
		D3D11_BLEND_DESC blendTransparentDesc;
		ZeroMemory(&blendTransparentDesc, sizeof(D3D11_BLEND_DESC));
		blendTransparentDesc.AlphaToCoverageEnable = FALSE;
		blendTransparentDesc.IndependentBlendEnable = FALSE;
		blendTransparentDesc.RenderTarget[0].BlendEnable = TRUE;
		blendTransparentDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		blendTransparentDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendTransparentDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendTransparentDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendTransparentDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		blendTransparentDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendTransparentDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
		blendTransparentDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		m_blendTransparent = new BlendState((ID3D11Device*)m_device, blendTransparentDesc);

		m_ResourceMgr = new ResourceMgr((ID3D11Device*)m_device, (ID3D11DeviceContext*)m_dContext);

		return 0;
	}

	int Graphic::Present()const
	{
		((ID3D11DeviceContext*)m_dContext)->RSSetViewports(1, (D3D11_VIEWPORT *)m_viewport);

		HRESULT hr = ((IDXGISwapChain*)m_swapchain)->Present(1, 0);
		if (FAILED(hr))
			return 1;

		const float black[4] = { 0.2,0.2,0.2,1 };
		((ID3D11DeviceContext*)m_dContext)->ClearRenderTargetView((ID3D11RenderTargetView*)m_rtv, black);
		((ID3D11DeviceContext*)m_dContext)->ClearDepthStencilView((ID3D11DepthStencilView*)m_dsView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		
		return 0;
	}

	Graphic::Graphic()
		:
		hwnd(nullptr),
		m_device(nullptr),
		m_dContext(nullptr),
		m_swapchain(nullptr),
		m_backBuffer(nullptr),
		m_depthStencilBuffer(nullptr),
		m_dsDepthEnabled(nullptr),
		m_dsDepthDisabled(nullptr),
		m_rsStatge(nullptr),
		m_blendOpaque(nullptr),
		m_blendTransparent(nullptr),

		m_rtv(nullptr),
		m_dsView(nullptr),
		m_ResourceMgr(nullptr),
		m_id(g_id++)
	{
	}

	Graphic::~Graphic()
	{
		((ID3D11Texture2D*)m_backBuffer)->Release();
		((ID3D11Device*)m_device)->Release();
		((ID3D11DeviceContext*)m_dContext)->Release();
		((IDXGISwapChain*)m_swapchain)->Release();

		((ID3D11RenderTargetView*)m_rtv)->Release();
		((ID3D11DepthStencilView*)m_dsView)->Release();
		((ID3D11Texture2D*)m_depthStencilBuffer)->Release();

		delete m_ResourceMgr;
	}

	int Graphic::SetPipelineData(PIP_REG id, const std::vector<const void*>& datas)
	{
		switch (id)
		{
		case PIP_REG::TRANSFORM_SHADER_VERTEX:
		{
			std::string* key = (std::string*)datas[0];

			VShader* vs;
			if (!m_ResourceMgr->HasVShader(*key))
			{
				std::string* path = (std::string*)datas[1];
				m_ResourceMgr->LoadVShader(*path);
			}
				
			if (!m_ResourceMgr->GetVShader(*key, &vs) && !vs->Apply((ID3D11DeviceContext*)m_dContext))
				return 1;
		}
		break;
		case PIP_REG::TRANSFORM_MESH:
		{
			std::string* key = (std::string*)datas[0];
			
			VertexBuffer* vb;
			IndexBuffer* ib;
			if (!m_ResourceMgr->HasMesh(*key))
			{
				const void* verticeData = datas[1];
				int* vertexCount = (int*)datas[2];
				const UINT* indiceData = (const UINT*)datas[3];
				int* indexCount = (int*)datas[4];
				m_ResourceMgr->LoadMesh(verticeData, *vertexCount, sizeof(DX::Vertex), indiceData, *indexCount, *key);
			}
			m_ResourceMgr->GetMesh(*key, &vb, &ib);

			vb->Apply((ID3D11DeviceContext*)m_dContext);
			ib->Apply((ID3D11DeviceContext*)m_dContext);
		}
		break;
		case PIP_REG::TRANSFORM_WORLD: 
		{
			int size = sizeof(float)*16;

			if (m_mCB.find(size) == m_mCB.end())
			{
				m_mCB[size] = new ConstantBuffer((ID3D11Device*)m_device, size);
			}
			m_mCB[size]->Write((ID3D11DeviceContext*)m_dContext, datas[0]);
			((ID3D11DeviceContext*)m_dContext)->VSSetConstantBuffers(SHADER_REG_CB_WORLD, 1, m_mCB[size]->GetAddress());
		}
		break;
		case PIP_REG::TRANSFORM_VIEW: 
		{
			int size = sizeof(float)*16;

			if (m_mCB.find(size) == m_mCB.end())
			{
				m_mCB[size] = new ConstantBuffer((ID3D11Device*)m_device, size);
			}
			m_mCB[size]->Write((ID3D11DeviceContext*)m_dContext, datas[0]);
			((ID3D11DeviceContext*)m_dContext)->VSSetConstantBuffers(SHADER_REG_CB_VIEW, 1, m_mCB[size]->GetAddress());
		}
		break;
		case PIP_REG::TRANSFORM_PROJECT: 
		{
			int size = sizeof(float)*16;

			if (m_mCB.find(size) == m_mCB.end())
			{
				m_mCB[size] = new ConstantBuffer((ID3D11Device*)m_device, size);
			}
			m_mCB[size]->Write((ID3D11DeviceContext*)m_dContext, datas[0]);
			((ID3D11DeviceContext*)m_dContext)->VSSetConstantBuffers(SHADER_REG_CB_PROJ, 1, m_mCB[size]->GetAddress());
		}
		break;
		case PIP_REG::DEPTH_ENABLE:
		{
			BOOL* bEnableDepth = (BOOL*)datas[0];

			if (*bEnableDepth)
			{
				m_dsDepthEnabled->Apply((ID3D11DeviceContext*)m_dContext);
			}
			else
			{
				m_dsDepthDisabled->Apply((ID3D11DeviceContext*)m_dContext);
			}
		}
		break;
		case PIP_REG::PIXEL_SHADER_PIXEL:
		{
			std::string* csoPath = (std::string*)datas[0];

			PShader* ps;
			if (!m_ResourceMgr->GetPShader(*csoPath, &ps))
				return 1;
			ps->Apply((ID3D11DeviceContext*)m_dContext);
		}
		break;
		case PIP_REG::PIXEL_LIGHT_DIRECTIONAL:
		{
			const void* lightData = datas[0];
			int size = *((int*)(datas[1]));

			if (m_mCB.find(size) == m_mCB.end())
			{
				m_mCB[size] = new ConstantBuffer((ID3D11Device*)m_device, size);
			}
			m_mCB[size]->Write((ID3D11DeviceContext*)m_dContext, lightData);
			((ID3D11DeviceContext*)m_dContext)->PSSetConstantBuffers(SHADER_REG_CB_LIGHT_DIRECTIONAL, 1, m_mCB[size]->GetAddress());
		}
		break;
		case PIP_REG::PIXEL_LIGHT_POINT:
		{
			const void* lightData = datas[0];
			int size = *((int*)(datas[1]));

			if (m_mCB.find(size) == m_mCB.end())
			{
				m_mCB[size] = new ConstantBuffer((ID3D11Device*)m_device, size);
			}
			m_mCB[size]->Write((ID3D11DeviceContext*)m_dContext, lightData);
			((ID3D11DeviceContext*)m_dContext)->PSSetConstantBuffers(SHADER_REG_CB_LIGHT_POINT, 1, m_mCB[size]->GetAddress());
		}
		break;
		case PIP_REG::PIXEL_LIGHT_SPOT:
		{
			const void* lightData = datas[0];
			int size = *((int*)(datas[1]));

			if (m_mCB.find(size) == m_mCB.end())
			{
				m_mCB[size] = new ConstantBuffer((ID3D11Device*)m_device, size);
			}
			m_mCB[size]->Write((ID3D11DeviceContext*)m_dContext, lightData);
			((ID3D11DeviceContext*)m_dContext)->PSSetConstantBuffers(SHADER_REG_CB_LIGHT_SPOT, 1, m_mCB[size]->GetAddress());
		}
		break;
		case PIP_REG::PIXEL_TEX:
		{
			std::vector< ID3D11ShaderResourceView*> srvs(10);
			for (int i=0; i< 10; ++i)
			{
				if (datas.size() <= i)
					break;

				srvs.push_back(nullptr);

				std::string* path = (std::string*)datas[i];
				ID3D11ShaderResourceView* srv;
				if (!m_ResourceMgr->HasTexture(*path))
				{
					if (!m_ResourceMgr->LoadTexture(*path))
						continue;
				}

				if (m_ResourceMgr->GetTexture(*path, &srv))
				{
					srvs[i] = srv;
				}
			}

			((ID3D11DeviceContext*)m_dContext)->PSSetShaderResources(SHADER_REG_SRV_TEX, 10, srvs.data());
		}
		break;
		case PIP_REG::PIXEL_EYE:
		{
			float* x = (float*)datas[0];
			float* y = (float*)datas[1];
			float* z = (float*)datas[2];
			float pos2[4] = { *x,*y,*z,0 };

			int size = sizeof(float) * 4;

			if (m_mCB.find(size) == m_mCB.end())
			{
				m_mCB[size] = new ConstantBuffer((ID3D11Device*)m_device, size);
			}
			m_mCB[size]->Write((ID3D11DeviceContext*)m_dContext, pos2);
			((ID3D11DeviceContext*)m_dContext)->VSSetConstantBuffers(SHADER_REG_CB_EYE, 1, m_mCB[size]->GetAddress());

		}
		break;
		case PIP_REG::BLENDING_OPAQUE:
		{
			m_blendOpaque->Apply((ID3D11DeviceContext*)m_dContext);
		}
		break;
		case PIP_REG::BLENDING_TRANSPARENT:
		{
			m_blendTransparent->Apply((ID3D11DeviceContext*)m_dContext);
		}
		break;
		default:
			return 1;
		}

		return 0;
	}
}
	


