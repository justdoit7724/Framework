#include "Mirror.h"
#include "Camera.h"
#include "Transform.h"
#include "Debugging.h"
#include "Quad.h"
#include "Shader.h"
#include "CameraMgr.h"

Mirror::Mirror(Scene* captureScene, UINT width, UINT height)
	:Object(
		nullptr,
		"MirrorVS.cso", simple_ILayouts, ARRAYSIZE(simple_ILayouts),
		"", "", "",
		"MirrorPS.cso",
		Z_ORDER_STANDARD),
	captureScene(captureScene),
	resolution(XMINT2(width, height))
{
	shape = new Quad();
	mirrorQuad[0] = XMFLOAT3(-0.5, -0.5, 0);
	mirrorQuad[1] = XMFLOAT3(-0.5, 0.5, 0);
	mirrorQuad[2] = XMFLOAT3(0.5, 0.5, 0);
	mirrorQuad[3] = XMFLOAT3(0.5, -0.5, 0);
	UINT OBJ_QUAD_INDICE[6] =
	{
		3,2,1,
		3,1,0
	};
	shape->Init(mirrorQuad, sizeof(mirrorQuad[0]), ARRAYSIZE(mirrorQuad), OBJ_QUAD_INDICE, ARRAYSIZE(OBJ_QUAD_INDICE), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	vs->AddCB(0, 1, sizeof(SHADER_TRANSFORMATION));
	ps->AddSRV(0, 1);
	ps->AddCB(0, 1, sizeof(float));
	float srvIdx = 0;
	ps->WriteCB(0, &srvIdx);
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

	perspective = new Camera("Mirror", CameraMgr::Instance()->Main());

	D3D11_TEXTURE2D_DESC capture_desc;
	capture_desc.ArraySize = 1;
	capture_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	capture_desc.CPUAccessFlags = 0;
	capture_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	capture_desc.Width = width;
	capture_desc.Height = height;
	capture_desc.MipLevels = 1;
	capture_desc.MiscFlags = 0;
	capture_desc.SampleDesc = { 1,0 };
	capture_desc.Usage = D3D11_USAGE_DEFAULT;
	ID3D11Texture2D* capture_tex;
	r_assert(
		DX_Device->CreateTexture2D(&capture_desc, nullptr, &capture_tex)
	);

	D3D11_RENDER_TARGET_VIEW_DESC rtv_desc;
	rtv_desc.Format = capture_desc.Format;
	rtv_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtv_desc.Texture2D.MipSlice = 0;
	r_assert(
		DX_Device->CreateRenderTargetView(capture_tex, &rtv_desc, rtv.GetAddressOf())
	);

	D3D11_TEXTURE2D_DESC ds_desc;
	ds_desc.ArraySize = 1;
	ds_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	ds_desc.CPUAccessFlags = 0;
	ds_desc.Format = DXGI_FORMAT_D32_FLOAT;
	ds_desc.Width = width;
	ds_desc.Height = height;
	ds_desc.MipLevels = 1;
	ds_desc.MiscFlags = 0;
	ds_desc.SampleDesc = { 1,0 };
	ds_desc.Usage = D3D11_USAGE_DEFAULT;
	ComPtr<ID3D11Texture2D> ds_tex;
	r_assert(
		DX_Device->CreateTexture2D(&ds_desc, nullptr, ds_tex.GetAddressOf())
	);
	D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc;
	dsv_desc.Format = ds_desc.Format;
	dsv_desc.Flags = 0;
	dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsv_desc.Texture2D.MipSlice = 0;
	r_assert(
		DX_Device->CreateDepthStencilView(ds_tex.Get(), &dsv_desc, dsv.GetAddressOf())
	);

	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
	srv_desc.Format = capture_desc.Format;
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srv_desc.Texture2DArray.ArraySize = 1;
	srv_desc.Texture2DArray.FirstArraySlice = 0;
	srv_desc.Texture2DArray.MipLevels = 1;
	srv_desc.Texture2DArray.MostDetailedMip = 0;
	r_assert(
		DX_Device->CreateShaderResourceView(capture_tex, &srv_desc, srv.GetAddressOf())
	);
}

void Mirror::Update(const Camera* camera, float elapsed, const XMMATRIX& texMat)
{
	UpdatePerspective(camera);

	ID3D11ShaderResourceView* nullSRV = nullptr;
	DX_DContext->PSSetShaderResources(0, 1, &nullSRV);
	DX_DContext->ClearRenderTargetView(rtv.Get(), Colors::Transparent);
	DX_DContext->ClearDepthStencilView(dsv.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	D3D11_VIEWPORT vp = CD3D11_VIEWPORT(0.0f,0.0f,resolution.x, resolution.y,0.0f,1.0f);
	UINT vpNum = 1;
	perspective->Capture(captureScene, rtv.GetAddressOf(), dsv.Get(), vp);

	vs->WriteCB(0, &(transform->WorldMatrix()*camera->VPMat(zOrder)));
	ps->WriteSRV(0, srv.Get());
}

void Mirror::UpdatePerspective(const Camera* eye)
{
	XMFLOAT3 mForward = transform->GetForward();
	XMFLOAT3 mUp = transform->GetUp();
	XMFLOAT3 mRight = transform->GetRight();
	XMFLOAT3 mC = transform->GetPos();
	float RU = mRight.x * mUp.x + mRight.y * mUp.y - mRight.z * mUp.z;
	float UF = mUp.x * mForward.x + mUp.y * mForward.y - mUp.z * mForward.z;
	float FR = mRight.x * mForward.x + mRight.y * mForward.y - mRight.z * mForward.z;
	float RR = mRight.x * mRight.x + mRight.y * mRight.y - mRight.z * mRight.z;
	float UU = mUp.x * mUp.x + mUp.y * mUp.y - mUp.z * mUp.z;
	float FF = mForward.x * mForward.x + mForward.y * mForward.y - mForward.z * mForward.z;
	XMFLOAT3 m = XMFLOAT3(

		-mC.x * mRight.x - mC.y * mUp.x - mC.z * mForward.x, 
		-mC.x * mRight.y - mC.y * mUp.y - mC.z * mForward.y, 
		mC.x * mRight.z + mC.y * mUp.z + mC.z * mForward.z);

	XMMATRIX perspectiveTranslationMat = XMMATRIX(
		RR, RU, FR, 0,
		RU, UU, UF, 0,
		FR, UF, FF, 0,
		Dot(m,mRight)+mC.x, Dot(m,mUp)+mC.y, Dot(m,mForward)+mC.z, 1);

	perspective->transform->SetTranslation(eye->transform->GetPos() * perspectiveTranslationMat);
	
	XMFLOAT3 ppos = perspective->transform->GetPos();
	XMFLOAT3 n = transform->GetForward();
	XMMATRIX perspectiveRotMat = XMMATRIX(
		-2 * n.x * n.x + 1, -2 * n.x * n.y, -2 * n.x * n.z,0,
		-2 * n.x * n.y, -2 * n.y * n.y + 1, -2 * n.y * n.z,0,
		-2 * n.x * n.z, -2 * n.y * n.z, -2 * n.z * n.z + 1,0,
		0,0,0,1
	);
	XMFLOAT3 look = eye->transform->GetForward()*perspectiveRotMat;
	XMFLOAT3 right = Cross(UP, look);
	XMFLOAT3 up = Cross(look,right);
	
	perspective->transform->SetRot(right, up, look);
}
