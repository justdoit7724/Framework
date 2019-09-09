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
		&DX_Device,
		NULL,
		&DX_DContext)
	);

	ComPtr<ID3D11Texture2D> backBuffer; 
	r_assert(
		swapchain->GetBuffer(
			0, 
			__uuidof(ID3D11Texture2D), 
			reinterpret_cast<void**>(backBuffer.GetAddressOf()))
	); 
	r_assert(
		DX_Device->CreateRenderTargetView(
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
		DX_Device->CreateTexture2D(
			&ds_desc,
			nullptr,
			depthStencilBuffer.GetAddressOf())
	);

	r_assert(
		DX_Device->CreateDepthStencilView(
			depthStencilBuffer.Get(),
			nullptr,
			dsView.GetAddressOf())
	);
	DX_DContext->OMSetRenderTargets(1, rtv.GetAddressOf(), dsView.Get());

	D3D11_DEPTH_STENCIL_DESC ds_desc2;
	ZeroMemory(&ds_desc2, sizeof(D3D11_DEPTH_STENCIL_DESC));
	ds_desc2.DepthEnable = true;
	ds_desc2.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	ds_desc2.DepthFunc = D3D11_COMPARISON_LESS;
	ds_desc2.StencilEnable = true;
	ds_desc2.StencilReadMask = 0xff;
	ds_desc2.StencilWriteMask = 0x01;
	D3D11_DEPTH_STENCILOP_DESC dsOp_desc;
	dsOp_desc.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsOp_desc.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsOp_desc.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dsOp_desc.StencilFunc = D3D11_COMPARISON_EQUAL;
	ds_desc2.FrontFace = dsOp_desc;
	ds_desc2.BackFace = dsOp_desc;
	
	r_assert(
		DX_Device->CreateDepthStencilState(
			&ds_desc2, dsState.GetAddressOf())
	);
	DX_DContext->OMSetDepthStencilState(dsState.Get(), 0);

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

	DX_DContext->RSSetViewports(1, &vp);
#pragma endregion

}

void Graphic::Present()
{
	swapchain->Present(1, 0);

	DX_DContext->ClearRenderTargetView(rtv.Get(), DirectX::Colors::Transparent);
	DX_DContext->ClearDepthStencilView(dsView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

	


