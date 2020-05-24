
#include "Graphic.h"

#pragma comment(lib,"d3d11.lib")

ID3D11Device* DX_Device=nullptr;
ID3D11DeviceContext* DX_DContext=nullptr;
float WND_WIDTH = -1.0f;
float WND_HEIGHT = -1.0f;

Graphic::Graphic(HWND _hwnd, float resX, float resY)
{
	hwnd = _hwnd;

	WND_WIDTH = resX;
	WND_HEIGHT = resY;

	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferDesc.Width = WND_WIDTH;
	scd.BufferDesc.Height = WND_HEIGHT;
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

	HRESULT hr=D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&scd,
		&swapchain,
		&DX_Device,
		NULL,
		&DX_DContext);
	r_assert(hr);

	hr = swapchain->GetBuffer(
			0,
			__uuidof(ID3D11Texture2D),
			reinterpret_cast<void**>(&backBuffer));
	r_assert(hr);

	hr = DX_Device->CreateRenderTargetView(
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
	hr = DX_Device->CreateRenderTargetView(
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

	hr = DX_Device->CreateTexture2D(
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
	hr = DX_Device->CreateDepthStencilView(
			depthStencilBuffer,
			&dsv_desc,
			&dsView);
	r_assert(hr);
	DX_DContext->OMSetRenderTargets(1, &rtv, dsView);

#pragma endregion

#pragma region Viewport

	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = resX;
	viewport.Height = resY;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	DX_DContext->RSSetViewports(1, &viewport);
#pragma endregion

#pragma region Rasterizer
	D3D11_RASTERIZER_DESC rs_desc;
	ZeroMemory(&rs_desc, sizeof(D3D11_RASTERIZER_DESC));
	rs_desc.FillMode = D3D11_FILL_SOLID;
	rs_desc.CullMode = D3D11_CULL_BACK;
	hr = DX_Device->CreateRasterizerState(&rs_desc, &rasterizerState);
	r_assert(hr);
	DX_DContext->RSSetState(rasterizerState);
#pragma endregion

}

Graphic::~Graphic()
{
	backBuffer->Release();
	depthStencilBuffer->Release();
	DX_DContext->Release();
	DX_Device->Release();

	swapchain->Release();
	rtv->Release();
	dsView->Release();
	rasterizerState->Release();
}

void Graphic::Present()
{
	swapchain->Present(1, 0);

	const float black[4] = { 0.1,0.1,0.1,1 };
	DX_DContext->ClearRenderTargetView(rtv, black);
	DX_DContext->ClearDepthStencilView(dsView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

	


