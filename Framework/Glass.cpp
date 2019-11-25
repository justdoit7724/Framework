#include "Glass.h"
#include "Camera.h"
#include "Transform.h"
#include "Shape.h"
#include "Shader.h"
#include "ShaderFormat.h"
#include "Scene.h"
#include "Game_info.h"

Glass::Glass(Scene* captureScene, Shape* shape)
	:Object(
		shape,
		"GlassVS.cso", Std_ILayouts, ARRAYSIZE(Std_ILayouts),
		"", "", "",
		"GlassPS.cso",
		Z_ORDER_STANDARD),
	captureScene(captureScene)
{
	vs->AddCB(0, 1, sizeof(XMMATRIX)*3);
	ps->AddSRV(0, 1);
	ps->AddCB(3, 1, sizeof(XMFLOAT4));
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(D3D11_SAMPLER_DESC));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	ps->AddSamp(0, 1, &sampDesc);

	const UINT captureWidth = SCREEN_WIDTH;
	const UINT captureHeight = SCREEN_HEIGHT;

	D3D11_TEXTURE2D_DESC capture_desc;
	capture_desc.Width = captureWidth;
	capture_desc.Height = captureHeight;
	capture_desc.MipLevels = 0;
	capture_desc.ArraySize = 6;
	capture_desc.SampleDesc = { 1,0 };
	capture_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	capture_desc.Usage = D3D11_USAGE_DEFAULT;
	capture_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	capture_desc.CPUAccessFlags = 0;
	capture_desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE|D3D11_RESOURCE_MISC_GENERATE_MIPS;
	ComPtr<ID3D11Texture2D> captureTex;
	r_assert(
		DX_Device->CreateTexture2D(&capture_desc, nullptr, captureTex.GetAddressOf())
	);

	D3D11_RENDER_TARGET_VIEW_DESC crtv_desc;
	crtv_desc.Format = capture_desc.Format;
	crtv_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
	crtv_desc.Texture2DArray.MipSlice = 0;
	crtv_desc.Texture2DArray.ArraySize = 1;
	for (int i = 0; i < 6; ++i)
	{
		crtv_desc.Texture2DArray.FirstArraySlice = i;
		r_assert(
			DX_Device->CreateRenderTargetView(captureTex.Get(), &crtv_desc, captureRTV[i].GetAddressOf())
		);
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
	srv_desc.Format = capture_desc.Format;
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	srv_desc.TextureCube.MostDetailedMip = 0;
	srv_desc.TextureCube.MipLevels = -1;
	r_assert(
		DX_Device->CreateShaderResourceView(captureTex.Get(), &srv_desc, &captureSRV)
	);

	D3D11_TEXTURE2D_DESC cds_desc;
	cds_desc.Width = captureWidth;
	cds_desc.Height = captureHeight;
	cds_desc.MipLevels = 1;
	cds_desc.ArraySize = 1;
	cds_desc.SampleDesc = { 1,0 };
	cds_desc.Format = DXGI_FORMAT_D32_FLOAT;
	cds_desc.Usage = D3D11_USAGE_DEFAULT;
	cds_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	cds_desc.CPUAccessFlags = 0;
	cds_desc.MiscFlags = 0;
	ComPtr<ID3D11Texture2D> cdsTex;
	r_assert(
		DX_Device->CreateTexture2D(&cds_desc, nullptr, &cdsTex)
	);
	D3D11_DEPTH_STENCIL_VIEW_DESC cdsv_desc;
	cdsv_desc.Format = cds_desc.Format;
	cdsv_desc.Flags = 0;
	cdsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	cdsv_desc.Texture2D.MipSlice = 0;
	r_assert(
		DX_Device->CreateDepthStencilView(cdsTex.Get(), &cdsv_desc, &captureDSV)
	);


	captureViewport.TopLeftX = 0.0f;
	captureViewport.TopLeftY = 0.0f;
	captureViewport.Width = captureWidth;
	captureViewport.Height = captureHeight;
	captureViewport.MinDepth = 0.0f;
	captureViewport.MaxDepth = 1.0f;

	captureCamera[0] = new Camera(FRAME_KIND_PERSPECTIVE, NULL, NULL, 1.0f, 100.0f, XM_PIDIV2, 1);
	captureCamera[0]->transform->SetRot(RIGHT, UP);
	captureCamera[1] = new Camera(FRAME_KIND_PERSPECTIVE, NULL, NULL, 1.0f, 100.0f, XM_PIDIV2, 1);
	captureCamera[1]->transform->SetRot(-RIGHT, UP);
	captureCamera[2] = new Camera(FRAME_KIND_PERSPECTIVE, NULL, NULL, 1.0f, 100.0f, XM_PIDIV2, 1);
	captureCamera[2]->transform->SetRot(UP, -FORWARD);
	captureCamera[3] = new Camera(FRAME_KIND_PERSPECTIVE, NULL, NULL, 1.0f, 100.0f, XM_PIDIV2, 1);
	captureCamera[3]->transform->SetRot(-UP, FORWARD);
	captureCamera[4] = new Camera(FRAME_KIND_PERSPECTIVE, NULL, NULL, 1.0f, 100.0f, XM_PIDIV2, 1);
	captureCamera[4]->transform->SetRot(FORWARD, UP);
	captureCamera[5] = new Camera(FRAME_KIND_PERSPECTIVE, NULL, NULL, 1.0f, 100.0f, XM_PIDIV2, 1);
	captureCamera[5]->transform->SetRot(-FORWARD, UP);
}

Glass::~Glass()
{
	for (int i = 0; i < 6; ++i)
	{
		delete captureCamera[i];
	}
}

void Glass::Render(const XMMATRIX& parentWorld, const XMMATRIX& vp, UINT sceneDepth) const
{
	XMMATRIX curWorld = transform->WorldMatrix()*parentWorld;

	if (sceneDepth > 0)
		return;

	//debug modify binding system
	ID3D11ShaderResourceView* const nullSRV = nullptr;
	DX_DContext->PSSetShaderResources(0, 1, &nullSRV);

	ID3D11RenderTargetView* oriRTV;
	ID3D11DepthStencilView* oriDSV;
	D3D11_VIEWPORT oriVP;
	DX_DContext->OMGetRenderTargets(1, &oriRTV, &oriDSV);
	UINT numVP = 1;
	DX_DContext->RSGetViewports(&numVP, &oriVP);

	for (int i = 0; i < 6; ++i)
	{
		captureCamera[i]->transform->SetTranslation(transform->GetPos());
		captureCamera[i]->Update();

		DX_DContext->ClearRenderTargetView(captureRTV[i].Get(), Colors::Transparent);
		DX_DContext->ClearDepthStencilView(captureDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		DX_DContext->OMSetRenderTargets(1, captureRTV[i].GetAddressOf(), captureDSV.Get());
		DX_DContext->RSSetViewports(1, &captureViewport);

		const XMMATRIX vp = captureCamera[i]->VMat() * captureCamera[i]->StdProjMat();
		const Frustum& frustum = captureCamera[i]->GetFrustum();
		captureScene->Render(vp, frustum, sceneDepth + 1);
	}
	DX_DContext->GenerateMips(captureSRV.Get());
	DX_DContext->OMSetRenderTargets(1, &oriRTV, oriDSV);
	DX_DContext->RSSetViewports(1, &oriVP);

	vs->WriteCB(0, &SHADER_STD_TRANSF(curWorld, vp));
	ps->WriteSRV(0, captureSRV.Get());

	Object::Render();
}