#include "Mirror.h"
#include "Camera.h"
#include "Transform.h"
#include "Debugging.h"
#include "Quad.h"
#include "Shader.h"

Mirror::Mirror(Scene* captureScene, UINT width, UINT height)
	:Object(
		new Quad(),
		"CMVS.cso", Std_ILayouts, ARRAYSIZE(Std_ILayouts),
		"", "", "",
		"CMPS.cso",
		Z_ORDER_STANDARD),
		captureScene(captureScene)
{
	vs->AddCB(0, 1, sizeof(XMMATRIX));
	ps->AddSRV(0, 1);
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

	perspective = new Camera("Mirror", FRAME_KIND_PERSPECTIVE, width, height, 0.01f, 1000.0f, 90.0f, width / (float)height, XMFLOAT3(0, 0, 0), FORWARD, UP);

	D3D11_TEXTURE2D_DESC capture_desc;
	capture_desc.ArraySize = 6;
	capture_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	capture_desc.CPUAccessFlags = 0;
	capture_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	capture_desc.Width = width;
	capture_desc.Height = height;
	capture_desc.MipLevels = 0;
	capture_desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
	capture_desc.SampleDesc = { 1,0 };
	capture_desc.Usage = D3D11_USAGE_DEFAULT;
	ID3D11Texture2D* capture_tex;
	r_assert(
		DX_Device->CreateTexture2D(&capture_desc, nullptr, &capture_tex)
	);

	D3D11_RENDER_TARGET_VIEW_DESC rtv_desc;
	rtv_desc.Format = capture_desc.Format;
	rtv_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
	rtv_desc.Texture2DArray.MipSlice = 0;
	rtv_desc.Texture2DArray.ArraySize = 1;
	for (int i = 0; i < 6; ++i)
	{
		rtv_desc.Texture2DArray.FirstArraySlice = i;
		r_assert(
			DX_Device->CreateRenderTargetView(capture_tex, &rtv_desc, rtv[i].GetAddressOf())
		);
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
	srv_desc.Format = capture_desc.Format;
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	srv_desc.TextureCube.MipLevels = -1;
	srv_desc.TextureCube.MostDetailedMip = 0;
	r_assert(
		DX_Device->CreateShaderResourceView(capture_tex, &srv_desc, srv.GetAddressOf())
	);

	ps->WriteSRV(0, srv.Get());
}

void Mirror::Update(const Camera* camera, const XMMATRIX& texMat)
{

	UpdatePerspective(camera);

	const XMMATRIX wvp = transform->WorldMatrix() * camera->VPMat(zOrder);
	vs->WriteCB(0, (void*)&wvp);

}

void Mirror::UpdatePerspective(const Camera* eye)
{
	XMFLOAT3 mForward = transform->GetForward();
	XMFLOAT3 mUp = transform->GetUp();
	XMFLOAT3 mRight = transform->GetRight();
	XMFLOAT3 mC = transform->GetPos();
	float  CR = Dot(mC, mRight);
	float  CU = Dot(mC, mUp);
	float  CF = Dot(mC, mForward);

	const XMMATRIX perspectiveMat = XMMATRIX(
		mRight.x * mRight.x + mUp.x * mUp.x - mForward.x * mForward.x, mRight.x * mRight.y + mUp.x * mUp.y - mForward.x * mForward.y, mRight.x * mRight.z + mUp.x * mUp.z - mForward.x * mForward.z, 0,
		mRight.x * mRight.y + mUp.x * mUp.y - mForward.x * mForward.y, mRight.y * mRight.y + mUp.y * mUp.y - mForward.y * mForward.y, mRight.y * mRight.z + mUp.y * mUp.z - mForward.y * mForward.z, 0,
		mRight.x * mRight.z + mUp.x * mUp.z - mForward.x * mForward.z, mRight.y * mRight.z + mUp.y * mUp.z - mForward.y * mForward.z, mRight.z * mRight.z + mUp.z * mUp.z - mForward.z * mForward.z, 0,
		-CR * mRight.x - CU * mUp.x - CF * mForward.x, -CR * mRight.y - CU * mUp.y - CF * mForward.y, -CR * mRight.z - CU * mUp.z - CF * mForward.z, 1);
	perspective->transform->SetTranslation(eye->transform->GetPos() * perspectiveMat);
	
	XMFLOAT3 look = Normalize(transform->GetPos() - perspective->transform->GetPos());
	XMFLOAT3 right = Cross(UP, look);
	XMFLOAT3 up = Cross(look,right);
	
	perspective->transform->SetRot(right, up, look);

	Debugging::Instance()->Mark(3, perspective->transform->GetPos(), 5);
	Debugging::Instance()->DirLine(0, perspective->transform->GetPos(), look, 20);
}
