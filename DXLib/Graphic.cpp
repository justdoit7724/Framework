

#include "pch.h"

#include "Graphic.h"
#include "Math.h"

namespace DX {

	Graphic::Graphic(HWND _hwnd, float resX, float resY)
	{
		XMFLOAT4 p(1, 1, 1, 1);
		const float n = 10;
		const float f = 100;
		XMMATRIX m1(
			n, 0.0f, 0.0f, 0.0f,
			0.0f, n, 0.0f, 0.0f,
			0, 0, (f + n) / (f - n), 1,
			0, 0, -2 * f * n / (f - n), 0
		); 
		XMMATRIX m2(
			n, 0, 0, 0,
			0, n, 0, 0,
			0, 0, 1, 1,
			0, 0, -2 * n, 0
		);

		auto r1 = Multiply(p, m1);
		auto r2 = Multiply(p, m2);

		hwnd = _hwnd;

		DXGI_SWAP_CHAIN_DESC scd;
		ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

		scd.BufferDesc.Width = (UINT)resX;
		scd.BufferDesc.Height = (UINT)resY;
		scd.BufferDesc.RefreshRate.Numerator = 60;
		scd.BufferDesc.RefreshRate.Denominator = 1;
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		scd.SampleDesc.Count = 1;
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
			&swapchain,
			&m_device,
			NULL,
			&m_dContext);
		r_assert(hr);

		hr = swapchain->GetBuffer(
			0,
			__uuidof(ID3D11Texture2D),
			reinterpret_cast<void**>(&backBuffer));
		r_assert(hr);

		hr = m_device->CreateRenderTargetView(
			backBuffer,
			nullptr,
			&rtv);
		r_assert(hr);

#pragma endregion

		hr = swapchain->GetBuffer(
			0,
			__uuidof(ID3D11Texture2D),
			reinterpret_cast<void**>(&backBuffer));
		r_assert(hr);
		hr = m_device->CreateRenderTargetView(
			backBuffer,
			nullptr,
			&rtv);
		r_assert(hr);

#pragma endregion

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

		hr = m_device->CreateTexture2D(
			&ds_desc,
			nullptr,
			&depthStencilBuffer);
		r_assert(hr);

		D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc;
		ZeroMemory(&dsv_desc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		dsv_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsv_desc.Flags = 0;
		dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
		dsv_desc.Texture2D.MipSlice = 0;
		hr = m_device->CreateDepthStencilView(
			depthStencilBuffer,
			&dsv_desc,
			&dsView);
		r_assert(hr);
		m_dContext->OMSetRenderTargets(1, &rtv, dsView);

#pragma endregion

#pragma region Viewport

		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = resX;
		viewport.Height = resY;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		m_dContext->RSSetViewports(1, &viewport);
#pragma endregion

#pragma region Rasterizer
		D3D11_RASTERIZER_DESC rs_desc;
		ZeroMemory(&rs_desc, sizeof(D3D11_RASTERIZER_DESC));
		rs_desc.FillMode = D3D11_FILL_SOLID;
		rs_desc.CullMode = D3D11_CULL_BACK;
		hr = m_device->CreateRasterizerState(&rs_desc, &rasterizerState);
		r_assert(hr);
		m_dContext->RSSetState(rasterizerState);
#pragma endregion

	}

	Graphic::~Graphic()
	{
		backBuffer->Release();
		depthStencilBuffer->Release();
		m_dContext->Release();
		m_device->Release();

		swapchain->Release();
		rtv->Release();
		dsView->Release();
		rasterizerState->Release();
	}

	void Graphic::Present()
	{
		swapchain->Present(1, 0);

		const float black[4] = { 0.1,0.1,0.1,1 };
		m_dContext->ClearRenderTargetView(rtv, black);
		m_dContext->ClearDepthStencilView(dsView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
	ID3D11Device* Graphic::Device()
	{
		return m_device;
	}
	ID3D11DeviceContext* Graphic::DContext()
	{
		return m_dContext;
	}
}
	


