#include "DynamicCubeMap.h"
#include "Camera.h"
#include "Transform.h"
#include "Shape.h"
#include "Shader.h"
#include "Network.h"
#include "CustomFormat.h"
#include "Scene.h"

DynamicCubeMap::DynamicCubeMap(IGraphic* graphic, Scene* captureScene, Shape* shape)
	:Object(
		shape,
		"CMVS.cso", Std_ILayouts,ARRAYSIZE(Std_ILayouts),
		"","","",
		"CMPS.cso",
		Z_ORDER_STANDARD),
	graphic(graphic),
	captureScene(captureScene)
{
	vs->AddCB(0, 1, sizeof(VS_Property));
	ps->AddSRV(0, 1);
	ps->AddCB(0, 1, sizeof(XMFLOAT3));
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

	const UINT captureLength = 1024;
	
	D3D11_TEXTURE2D_DESC capture_desc;
	capture_desc.Width = captureLength;
	capture_desc.Height = captureLength;
	capture_desc.MipLevels = 0;
	capture_desc.ArraySize = 6;
	capture_desc.SampleDesc = { 1,0 };
	capture_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	capture_desc.Usage = D3D11_USAGE_DEFAULT;
	capture_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	capture_desc.CPUAccessFlags = 0;
	capture_desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
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

	D3D11_TEXTURE2D_DESC cds_desc;
	cds_desc.Width = captureLength;
	cds_desc.Height = captureLength;
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
	cdsv_desc.Texture2D.MipSlice=0;
	r_assert(
		DX_Device->CreateDepthStencilView(cdsTex.Get(), &cdsv_desc, &captureDSV)
	);

	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
	srv_desc.Format = capture_desc.Format;
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	srv_desc.TextureCube.MostDetailedMip = 0;
	srv_desc.TextureCube.MipLevels = 1;
	r_assert(
		DX_Device->CreateShaderResourceView(captureTex.Get(), &srv_desc, &captureSRV)
	);


	captureViewport.TopLeftX = 0.0f;
	captureViewport.TopLeftY = 0.0f;
	captureViewport.Width = captureLength;
	captureViewport.Height = captureLength;
	captureViewport.MinDepth = 0.0f;
	captureViewport.MaxDepth = 1.0f;

	captureCamera[0] = new Camera("capture0", FRAME_KIND_PERSPECTIVE, captureLength, captureLength, 100.0f, 1000.0f, XM_PIDIV2, 1, XMFLOAT3(0,0,0), RIGHT, UP);
	captureCamera[1] = new Camera("capture1", FRAME_KIND_PERSPECTIVE, captureLength, captureLength, 100.0f, 1000.0f, XM_PIDIV2, 1, XMFLOAT3(0,0,0), -RIGHT, UP);
	captureCamera[2] = new Camera("capture2", FRAME_KIND_PERSPECTIVE, captureLength, captureLength, 100.0f, 1000.0f, XM_PIDIV2, 1, XMFLOAT3(0,0,0), UP, -FORWARD);
	captureCamera[3] = new Camera("capture3", FRAME_KIND_PERSPECTIVE, captureLength, captureLength, 100.0f, 1000.0f, XM_PIDIV2, 1, XMFLOAT3(0,0,0), -UP, FORWARD);
	captureCamera[4] = new Camera("capture4", FRAME_KIND_PERSPECTIVE, captureLength, captureLength, 100.0f, 1000.0f, XM_PIDIV2, 1, XMFLOAT3(0,0,0), FORWARD, UP);
	captureCamera[5] = new Camera("capture5", FRAME_KIND_PERSPECTIVE, captureLength, captureLength, 100.0f, 1000.0f, XM_PIDIV2, 1, XMFLOAT3(0,0,0), -FORWARD, UP);
}

void DynamicCubeMap::Update(const Camera* camera, const XMMATRIX& texMat)
{
	ID3D11ShaderResourceView* const nullSRV = nullptr;
	DX_DContext->PSSetShaderResources(0, 1, &nullSRV);
	for (int i = 0; i < 6; ++i)
	{
		captureCamera[i]->transform->SetTranslation(transform->GetPos());

		DX_DContext->ClearRenderTargetView(captureRTV[i].Get(), Colors::Transparent);
		DX_DContext->ClearDepthStencilView(captureDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		captureCamera[i]->Capture(captureScene, captureRTV[i].GetAddressOf(), captureDSV.Get(), captureViewport);
	}

	vs->WriteCB(0, &VS_Property(transform->WorldMatrix(), camera->VPMat(zOrder), XMMatrixIdentity()));
	ps->WriteSRV(0, captureSRV.Get());
	ps->WriteCB(0, &(camera->transform->GetPos()));
}