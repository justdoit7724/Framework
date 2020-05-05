#include "PlayScene.h"
#include "Object.h"
#include "SphereMesh.h"
#include "CylinderMesh.h"
#include "TextureMgr.h"
#include "Transform.h"
#include "Light.h"
#include "CameraMgr.h"
#include "Buffer.h"
#include "Camera.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Debugging.h"
#include "LineMesh.h"
#include "ShaderFormat.h"
#include "Geometrics.h"
#include "UnlitObj.h"
#include "UI.h"
#include "MeshLoader.h"
#include "ShaderReg.h"

PlayScene::PlayScene()
	:camDist(300), camViewRad(XM_PIDIV2)
{
	dLight = new DirectionalLight(
		XMFLOAT3(0.5, 0.5, 0.5),
		XMFLOAT3(0.8, 0.8, 0.8),
		XMFLOAT3(0.4, 0.4, 0.4),
		Normalize(XMFLOAT3(-1,-0.5f,-1))
	);

#pragma region dir interface

	auto barMesh = std::make_shared<CylinderMesh>(12);
	iXBar = new UnlitObj("I x bar", barMesh, nullptr, Colors::Red, false);
	iYBar = new UnlitObj("I y bar", barMesh, nullptr, Colors::Green, false);
	iZBar = new UnlitObj("I z bar", barMesh, nullptr, Colors::Blue, false);
	iXBar->transform->SetTranslation(40, 0, 0);
	iYBar->transform->SetTranslation(0, 40, 0);
	iZBar->transform->SetTranslation(0, 0, 40);
	iXBar->transform->SetRot(UP, RIGHT);
	iYBar->transform->SetRot(FORWARD, UP);
	iZBar->transform->SetRot(UP, FORWARD);
	iXBar->transform->SetScale(10, 80, 10);
	iYBar->transform->SetScale(10, 80, 10);
	iZBar->transform->SetScale(10, 80, 10);
	iXBar->SetLayer(LAYER_INTERFACE_CAMERA);
	iYBar->SetLayer(LAYER_INTERFACE_CAMERA);
	iZBar->SetLayer(LAYER_INTERFACE_CAMERA);

	iVP.TopLeftX = 0;
	iVP.TopLeftY = 0;
	iVP.Width = 130;
	iVP.Height = 130;
	iVP.MinDepth = 0;
	iVP.MaxDepth = 1;
	D3D11_TEXTURE2D_DESC itex_desc;
	itex_desc.Width = iVP.Width;
	itex_desc.Height = iVP.Height;
	itex_desc.MipLevels = 1;
	itex_desc.ArraySize = 1;
	itex_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	itex_desc.SampleDesc = { 1,0 };
	itex_desc.Usage = D3D11_USAGE_DEFAULT;
	itex_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	itex_desc.CPUAccessFlags = 0;
	itex_desc.MiscFlags = 0;
	ComPtr<ID3D11Texture2D> iTex;
	HRESULT hr = DX_Device->CreateTexture2D(&itex_desc, nullptr, iTex.GetAddressOf());
	r_assert(hr);
	D3D11_RENDER_TARGET_VIEW_DESC rtv_desc;
	rtv_desc.Format = itex_desc.Format;
	rtv_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtv_desc.Texture2D.MipSlice = 0;
	hr = DX_Device->CreateRenderTargetView(iTex.Get(), &rtv_desc, iRTV.GetAddressOf());
	r_assert(hr);
	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
	srv_desc.Format = itex_desc.Format;
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv_desc.Texture2D.MipLevels = 1;
	srv_desc.Texture2D.MostDetailedMip = 0;
	hr = DX_Device->CreateShaderResourceView(iTex.Get(), &srv_desc, iSRV.GetAddressOf());
	r_assert(hr);
	D3D11_TEXTURE2D_DESC ds_desc;
	ds_desc.Width = iVP.Width;
	ds_desc.Height = iVP.Height;
	ds_desc.MipLevels = 1;
	ds_desc.ArraySize = 1;
	ds_desc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	ds_desc.SampleDesc.Count = 1;
	ds_desc.SampleDesc.Quality = 0;
	ds_desc.Usage = D3D11_USAGE_DEFAULT;
	ds_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	ds_desc.CPUAccessFlags = 0;
	ds_desc.MiscFlags = 0;
	ComPtr<ID3D11Texture2D> iDTex;
	hr = DX_Device->CreateTexture2D(
		&ds_desc,
		nullptr,
		&iDTex);
	r_assert(hr);

	D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc;
	ZeroMemory(&dsv_desc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	dsv_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsv_desc.Flags = 0;
	dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsv_desc.Texture2D.MipSlice = 0;
	hr = DX_Device->CreateDepthStencilView(
		iDTex.Get(),
		&dsv_desc,
		iDSV.GetAddressOf());
	r_assert(hr);

	canvas = new UICanvas();
	new UI(XMFLOAT2(0, 0), XMFLOAT2(iVP.Width, iVP.Height), 0, iSRV.Get());

	iCam = new Camera("interface Cam", FRAME_KIND_PERSPECTIVE, WND_WIDTH, WND_HEIGHT, 1.0f, 1000.0f, camViewRad, 1, false);
	iCam->transform->SetTranslation(0, 0, -10);
	iCam->SetLayer(LAYER_INTERFACE_CAMERA);
#pragma endregion

	

	checkCam = new Camera("CheckCam", FRAME_KIND_PERSPECTIVE, NULL, NULL, 1.0f, 1000.0f, camViewRad, 1, true);
	checkCam->transform->SetTranslation(0, 0, -camDist);
	checkCam->SubtractLayer(LAYER_INTERFACE_CAMERA);
	checkCam->SubtractLayer(LAYER_UI);
	CameraMgr::Instance()->SetMain(checkCam);
	Debugging::Instance()->SetCamera(checkCam);


	cbEye = new Buffer(sizeof(XMFLOAT4));

	TextureMgr::Instance()->Load("white", "DXFramework\\Data\\Texture\\white.png");
	TextureMgr::Instance()->Load("green", "DXFramework\\Data\\Texture\\green_light.png");
	TextureMgr::Instance()->Load("sample", "DXFramework\\Data\\Texture\\sample.jpg");

	CreateModel("Hand.ply");
	CreateGrid();
}

void PlayScene::Update(float elapsed, float spf)
{
	XMFLOAT3 eye = CameraMgr::Instance()->Main()->transform->GetPos();
	dLight->SetDir(Normalize(XMFLOAT3(cos(elapsed/10.0f), -0.35f, sin(elapsed/10.0f))));


	if (Keyboard::GetKey('A')==KeyState_Up)
	{
		Lerp2CamX();
	}
	else if (Keyboard::GetKey('W') == KeyState_Up)
	{
		Lerp2CamY();
	}
	else if (Keyboard::GetKey('D') == KeyState_Up)
	{
		Lerp2CamZ();
	}

	CamMove(spf);

	Render2Texture();

	cbEye->Write(&eye);
	DX_DContext->PSSetConstantBuffers(SHADER_REG_CB_EYE, 1, cbEye->GetAddress());
	dLight->Apply();
}

void PlayScene::CreateModel(std::string filename)
{
	if (mainObj)
		delete mainObj;

	std::vector<Mesh*> meshes;
	MeshLoader::Load(&meshes, "DXFramework\\Data\\Model\\PLY\\", filename);

	mainObj = new Object("Test Obj", std::shared_ptr<Mesh>(meshes[0]), nullptr, TextureMgr::Instance()->Get("white"));
	mainObj->Update();
}

void PlayScene::CreateGrid()
{
	if (grid3D)
	{
		delete grid3D;
	}

	float lowY = mainObj? mainObj->Bound().p.y - mainObj->Bound().rad : 0;
	const float length = 300;
	const int gridCount = 50;
	const float interval = length / gridCount;
	const XMFLOAT3 startP = XMFLOAT3(-gridCount / 2.0f, 0, -gridCount / 2.0f)*interval;
	auto lineMesh = std::make_shared<LineMesh>();
	for (int x = 0; x < gridCount; ++x)
	{
		XMFLOAT3 p1 = startP + XMFLOAT3(x, 0, 0)*interval;
		XMFLOAT3 p2 = startP + XMFLOAT3(x, 0, gridCount)*interval;
		p1.y += lowY;
		p2.y += lowY;
		lineMesh->Add(p1, p2);
	}
	for (int z = 0; z < gridCount; ++z)
	{
		XMFLOAT3 p1 = startP + XMFLOAT3(0, 0, z)*interval;
		XMFLOAT3 p2 = startP + XMFLOAT3(gridCount, 0, z)*interval;
		p1.y += lowY;
		p2.y += lowY;
		lineMesh->Add(p1, p2);
	}
	lineMesh->Generate();
	grid3D = new UnlitObj("3D grid", lineMesh, nullptr, Colors::White,true);
}

void PlayScene::Lerp2CamX()
{
	if (isLerping)
		return;
	isLerping = true;
	curLerpTime = 0;
	startAngleX = camAngleX;
	startAngleY = camAngleY;
	lerpAngleX = 0;
	lerpAngleY = -XM_PIDIV4 * 2;
	XMFLOAT2 topdownPos = XMFLOAT2(checkCam->transform->GetPos().x, checkCam->transform->GetPos().z);
	if (Dot(topdownPos, XMFLOAT2(0,1)) > 0 && startAngleY >= 0)
	{
		lerpAngleY = XM_PIDIV4 * 6;
	}
}

void PlayScene::Lerp2CamY()
{
	if (isLerping)
		return;
	isLerping = true;
	curLerpTime = 0;
	startAngleX = camAngleX;
	startAngleY = camAngleY;
	lerpAngleX = XM_PIDIV2*0.99f;
	lerpAngleY = camAngleY;

}

void PlayScene::Lerp2CamZ()
{
	if (isLerping)
		return;
	isLerping = true;
	curLerpTime = 0;
	startAngleX = camAngleX;
	startAngleY = camAngleY;
	lerpAngleX = 0;
	lerpAngleY = -XM_PIDIV4 * 4;
	XMFLOAT2 topdownPos = XMFLOAT2(checkCam->transform->GetPos().x, checkCam->transform->GetPos().z);
	if (Dot(topdownPos, XMFLOAT2(-1, 0)) > 0 && startAngleY >= 0)
	{
		lerpAngleY = XM_PIDIV4 * 4;
	}
	else
	{
		if (startAngleY >= 0)
			lerpAngleY = XM_PIDIV4 * 4;
		else
			lerpAngleY = -XM_PIDIV4 * 4;
	}
}

void PlayScene::Lerp2Perspective()
{
	if (isLerping)
		return;
	isLerping = true;
	curLerpTime = 0;
}

void PlayScene::Lerp2Orthogonal()
{
	if (isLerping)
		return;
	isLerping = true;
	curLerpTime = 0;
}

void PlayScene::CamMove(float spf)
{


	if (!isLerping)
	{
		XMFLOAT3 right = checkCam->transform->GetRight();
		XMFLOAT3 forward = checkCam->transform->GetForward();
		const float speed = 30;

		static XMFLOAT2 prevMousePt = XMFLOAT2(0, 0);
		const float angleSpeed = 3.141592f * 0.25f;
		XMFLOAT2 mPt = Mouse::Instance()->Pos();
		switch (Mouse::Instance()->RightState())
		{
		case MOUSE_STATE_PRESSING:
			camAngleY = camAngleY + angleSpeed * spf * (mPt.x - prevMousePt.x);
			camAngleX = Clamp(-XM_PIDIV2*0.99f, XM_PIDIV2*0.99f, camAngleX + angleSpeed * spf * (mPt.y - prevMousePt.y));
			break;
		}
		if (camAngleY > XM_PI*2) camAngleY -= XM_PI*2;
		if (camAngleY < -XM_PI*2) camAngleY += XM_PI*2;

		camViewRad = Clamp(XM_PI * 0.1f, XM_PI * 0.6f, checkCam->GetVRad() - Mouse::Instance()->GetWheel() * 0.1f);
		prevMousePt.x = mPt.x;
		prevMousePt.y = mPt.y;
	}
	else
	{
		curLerpTime += spf;
		float t = min(curLerpTime / lerpDuration, 1);

		camAngleX = Lerp(startAngleX, lerpAngleX, t);
		camAngleY = Lerp(startAngleY, lerpAngleY, t);

		if (t >= 1)
		{
			isLerping = false;
		}

		
	}

	XMMATRIX rotY(
		cos(camAngleY), 0, -sin(camAngleY), 0,
		0, 1, 0, 0,
		sin(camAngleY), 0, cos(camAngleY), 0,
		0, 0, 0, 1
	);
	XMFLOAT3 newPos = Multiply(XMFLOAT3(0, sin(camAngleX), -cos(camAngleX)) * camDist, rotY);

	XMFLOAT3 newForward = -Normalize(newPos);
	XMFLOAT3 newRight = Normalize(Cross(UP, newForward));
	XMFLOAT3 newUp = Cross(newForward, newRight);

	checkCam->SetFrame(FRAME_KIND_PERSPECTIVE, XMFLOAT2(WND_WIDTH, WND_HEIGHT), checkCam->GetN(), checkCam->GetF(), camViewRad, checkCam->GetAspectRatio());
	checkCam->transform->SetTranslation(newPos);
	checkCam->transform->SetRot(newForward, newUp, newRight);
	checkCam->Update();
	iCam->SetFrame(FRAME_KIND_PERSPECTIVE, XMFLOAT2(WND_WIDTH, WND_HEIGHT), iCam->GetN(), iCam->GetF(), XM_PIDIV4*0.8f, iCam->GetAspectRatio());
	iCam->transform->SetTranslation(newPos);
	iCam->transform->SetRot(newForward, newUp, newRight);
	iCam->Update();
}

void PlayScene::Render2Texture()
{
	ID3D11RenderTargetView* oriRTV=nullptr;
	ID3D11DepthStencilView* oriDSV=nullptr;
	D3D11_VIEWPORT oriVP;
	UINT num=1;
	DX_DContext->RSGetViewports(&num, &oriVP);
	DX_DContext->OMGetRenderTargets(1, &oriRTV, &oriDSV);

	DX_DContext->RSSetViewports(1, &iVP);


	DX_DContext->ClearRenderTargetView(iRTV.Get(), Colors::Black);
	DX_DContext->ClearDepthStencilView(iDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, NULL);
	ID3D11ShaderResourceView* nullSRV = nullptr;
	DX_DContext->PSSetShaderResources(SHADER_REG_SRV_DIFFUSE, 0, &nullSRV);
	DX_DContext->OMSetRenderTargets(1, iRTV.GetAddressOf(), iDSV.Get());

	XMMATRIX vp = iCam->VMat()* iCam->ProjMat();
	iXBar->Render(vp, iCam->GetFrustum(), 0);
	iYBar->Render(vp, iCam->GetFrustum(), 0);
	iZBar->Render(vp, iCam->GetFrustum(), 0);

	DX_DContext->RSSetViewports(1, &oriVP);
	DX_DContext->OMSetRenderTargets(1, &oriRTV, oriDSV);
}
