#include "Graphic.h"
#include "Game_info.h"

#pragma comment(lib, "d3d11.lib")

ID3D11Device* DX_Device=nullptr;
ID3D11DeviceContext* DX_DContext=nullptr;

Graphic::Graphic(HWND _hwnd)
{
	hwnd = _hwnd;
	
#pragma region Device / Context / SwapChain

	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferDesc.Width = SCREEN_WIDTH;
	scd.BufferDesc.Height = SCREEN_HEIGHT;
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	scd.SampleDesc.Count = 8;
	scd.SampleDesc.Quality = 0;

	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.BufferCount = 1;
	scd.OutputWindow = hwnd;
	scd.Windowed = TRUE;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	r_assert(
		D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&scd,
		swapchain.GetAddressOf(),
		&DX_Device,
		NULL,
		&DX_DContext)
	);

	r_assert(
		swapchain->GetBuffer(
			0, 
			__uuidof(ID3D11Texture2D), 
			reinterpret_cast<void**>(&backBuffer))
	); 
	r_assert(
		DX_Device->CreateRenderTargetView(
			backBuffer, 
			nullptr, 
			rtv.GetAddressOf())
	);

#pragma endregion

#pragma region Depth&Stencil Buffer

	D3D11_TEXTURE2D_DESC ds_desc;
	ds_desc.Width = SCREEN_WIDTH;
	ds_desc.Height = SCREEN_HEIGHT;
	ds_desc.MipLevels = 1;
	ds_desc.ArraySize = 1;
	// TYPELESS instead of UNORM for copying data
	ds_desc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	ds_desc.SampleDesc = scd.SampleDesc;
	ds_desc.Usage = D3D11_USAGE_DEFAULT;
	ds_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	ds_desc.CPUAccessFlags = 0;
	ds_desc.MiscFlags = 0;

	r_assert(
		DX_Device->CreateTexture2D(
			&ds_desc,
			nullptr,
			&depthStencilBuffer)
	);

	D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc;
	dsv_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsv_desc.Flags = 0;
	dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	dsv_desc.Texture2D.MipSlice = 0;
	r_assert(
		DX_Device->CreateDepthStencilView(
			depthStencilBuffer,
			&dsv_desc,
			dsView.GetAddressOf())
	);
	DX_DContext->OMSetRenderTargets(1, rtv.GetAddressOf(), dsView.Get());

#pragma endregion

#pragma region Viewport

	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = SCREEN_WIDTH;
	viewport.Height = SCREEN_HEIGHT;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	DX_DContext->RSSetViewports(1, &viewport);
#pragma endregion

#pragma region Rasterizer
	D3D11_RASTERIZER_DESC rs_desc;
	ZeroMemory(&rs_desc, sizeof(D3D11_RASTERIZER_DESC));
	rs_desc.FillMode = D3D11_FILL_SOLID;
	rs_desc.CullMode = D3D11_CULL_BACK;
	r_assert(
		DX_Device->CreateRasterizerState(&rs_desc, rasterizerState.GetAddressOf())
	);
	DX_DContext->RSSetState(rasterizerState.Get());
#pragma endregion

}

Graphic::~Graphic()
{
	backBuffer->Release();
	depthStencilBuffer->Release();
}

void Graphic::Present()
{
	swapchain->Present(1, 0);

	DX_DContext->ClearRenderTargetView(rtv.Get(), DirectX::Colors::Transparent);
	DX_DContext->ClearDepthStencilView(dsView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

	


