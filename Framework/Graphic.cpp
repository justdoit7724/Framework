#include "Graphic.h"
#include "Game_info.h"

#pragma comment(lib, "d3d11.lib")

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

	scd.SampleDesc.Count = 1;
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
		device.GetAddressOf(),
		NULL,
		dContext.GetAddressOf())
	);

	ComPtr<ID3D11Texture2D> backBuffer; 
	r_assert(
		swapchain->GetBuffer(
			0, 
			__uuidof(ID3D11Texture2D), 
			reinterpret_cast<void**>(backBuffer.GetAddressOf()))
	); 
	r_assert(
		device->CreateRenderTargetView(
			backBuffer.Get(), 
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
	ds_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	ds_desc.SampleDesc.Count = 1;
	ds_desc.SampleDesc.Quality = 0;
	ds_desc.Usage = D3D11_USAGE_DEFAULT;
	ds_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	ds_desc.CPUAccessFlags = 0;
	ds_desc.MiscFlags = 0;

	ComPtr<ID3D11Texture2D> depthStencilBuffer;
	r_assert(
		device->CreateTexture2D(
			&ds_desc,
			nullptr,
			depthStencilBuffer.GetAddressOf())
	);

	r_assert(
		device->CreateDepthStencilView(
			depthStencilBuffer.Get(),
			nullptr,
			dsView.GetAddressOf())
	);
	dContext->OMSetRenderTargets(1, rtv.GetAddressOf(), dsView.Get());
	//dContext->OMSetRenderTargets(1, &rtvs[1], dsView.Get());

	D3D11_DEPTH_STENCIL_DESC ds_desc2;
	ZeroMemory(&ds_desc2, sizeof(D3D11_DEPTH_STENCIL_DESC));
	ds_desc2.DepthEnable = true;
	ds_desc2.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	ds_desc2.DepthFunc = D3D11_COMPARISON_LESS;
	ds_desc2.StencilEnable = false;

	r_assert(
		device->CreateDepthStencilState(
			&ds_desc2, dsState.GetAddressOf())
	);
	dContext->OMSetDepthStencilState(dsState.Get(), 0);

#pragma endregion

#pragma region Viewport

	D3D11_VIEWPORT vp;
	ZeroMemory(&vp, sizeof(D3D11_VIEWPORT));
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = SCREEN_WIDTH;
	vp.Height = SCREEN_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	dContext->RSSetViewports(1, &vp);
#pragma endregion

#pragma region Rasterizer
	D3D11_RASTERIZER_DESC rs_desc;
	ZeroMemory(&rs_desc, sizeof(D3D11_RASTERIZER_DESC));
	rs_desc.FillMode = D3D11_FILL_SOLID;
	rs_desc.CullMode = D3D11_CULL_BACK;
	r_assert(
		device->CreateRasterizerState(&rs_desc, rasterizerState.GetAddressOf())
	);
	dContext->RSSetState(rasterizerState.Get());
#pragma endregion

}

void Graphic::Present()
{
	swapchain->Present(1, 0);

	dContext->ClearRenderTargetView(rtv.Get(), DirectX::Colors::Transparent);
	dContext->ClearDepthStencilView(dsView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

	


