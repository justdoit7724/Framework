#include "stdafx.h"
#include "StartScene.h"
#include "Keyboard.h"
#include "Timer.h"

using namespace DX;

StartScene::StartScene(ID3D11Device* device, ID3D11DeviceContext* dContext, const wchar_t* key)
	:Scene(device, dContext, key)
{
	m_keyboard = new Keyboard();

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(D3D11_SAMPLER_DESC));
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	FLOAT black[4] = { 0,0,0,1 };
	sampDesc.BorderColor[0] = 0;
	sampDesc.BorderColor[1] = 0;
	sampDesc.BorderColor[2] = 0;
	sampDesc.BorderColor[3] = 1;
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	ID3D11SamplerState* samp;
	m_device->CreateSamplerState(&sampDesc, &samp);
	m_dContext->PSSetSamplers(SHADER_REG_SAMP_POINT, 1, &samp);
	samp->Release();

	m_dLight = new DX::DirectionalLight(
		XMFLOAT3(0.25, 0.25, 0.25),
		XMFLOAT3(0.8, 0.8, 0.8),
		XMFLOAT3(1.0, 1.0, 1.0),
		DX::Normalize(XMFLOAT3(-1, -0.5f, -1))
	);

	m_camera = new DX::Camera("cam", DX::FRAME_KIND_PERSPECTIVE, NULL, NULL, 1.0f, 1000.0f, XM_PIDIV2, 1, false);
	m_camera->transform->SetTranslation(0, 0, -50);

	m_cbEye = new DX::Buffer(device, sizeof(XMFLOAT4));

	std::shared_ptr<DX::Mesh> pSphereMesh = std::make_shared<DX::SphereMesh>(m_device,4);
	std::shared_ptr<DX::Mesh> pCubeMesh = std::make_shared<DX::CubeMesh>(m_device);
	std::shared_ptr<DX::Mesh> pFloorMesh = std::make_shared<DX::QuadMesh>(m_device);


	ID3D11ShaderResourceView* redSRV;
	ID3D11ShaderResourceView* blueSRV;
	ID3D11ShaderResourceView* greenSRV;
	ID3D11ShaderResourceView* whiteSRV;
	ID3D11ShaderResourceView* normSRV;
	DX::LoadTexture(m_device, m_dContext, "C:\\Users\\Jun\\source\\repos\\Framework\\Data\\Texture\\red_light.png", &redSRV);
	DX::LoadTexture(m_device, m_dContext, "C:\\Users\\Jun\\source\\repos\\Framework\\Data\\Texture\\green_light.png", &greenSRV);
	DX::LoadTexture(m_device, m_dContext, "C:\\Users\\Jun\\source\\repos\\Framework\\Data\\Texture\\blue_light.png", &blueSRV);
	DX::LoadTexture(m_device, m_dContext, "C:\\Users\\Jun\\source\\repos\\Framework\\Data\\Texture\\white.png", &whiteSRV);
	DX::LoadTexture(m_device, m_dContext, "C:\\Users\\Jun\\source\\repos\\Framework\\Data\\Texture\\default_normal.png", &normSRV);


	m_dxBlueBox = new DX::Object(m_device, m_dContext, "obj", pCubeMesh, nullptr, redSRV, normSRV);
	m_dxBlueBox->transform->SetScale(40, 40, 20);
	m_dxBlueBox->transform->SetTranslation(0,20,50);
	m_vObj.push_back(m_dxBlueBox);

	m_dxBlueBox = new DX::Object(m_device, m_dContext, "obj", pSphereMesh, nullptr, greenSRV, normSRV);
	m_dxBlueBox->transform->SetScale(20, 20, 20);
	m_dxBlueBox->transform->SetTranslation(40,40,50);
	m_vObj.push_back(m_dxBlueBox);

	m_dxBlueBox = new DX::Object(m_device, m_dContext, "obj", pCubeMesh, nullptr, blueSRV, normSRV);
	m_dxBlueBox->transform->SetScale(30, 20, 30);
	m_dxBlueBox->transform->SetTranslation(-40,30,50);
	m_vObj.push_back(m_dxBlueBox);

	DX::Object* dxFloor = new DX::Object(m_device, m_dContext, "obj", pFloorMesh, nullptr, whiteSRV, normSRV);
	dxFloor->transform->SetScale(200, 200, 1);
	dxFloor->transform->SetRot(UP);
	m_vObj.push_back(dxFloor);


	/*
	TextureMgr::Instance()->Load("white", "DXFramework\\Data\\Texture\\white.png");
	TextureMgr::Instance()->Load("green", "DXFramework\\Data\\Texture\\green_light.png");
	TextureMgr::Instance()->Load("sample", "DXFramework\\Data\\Texture\\sample.jpg");*/
}

StartScene::~StartScene()
{
	delete m_dLight;
	delete m_camera;
	delete m_cbEye;
	delete m_keyboard;
}

void StartScene::Update(float elapsed, float spf)
{
	XMFLOAT3 eye = m_camera->transform->GetPos();

	m_cbEye->Write(m_dContext, &eye);
	m_dContext->PSSetConstantBuffers(SHADER_REG_CB_EYE, 1, m_cbEye->GetAddress());
	m_dLight->Apply(m_device,m_dContext);
	

	XMFLOAT3 newPos = m_camera->transform->GetPos();
	XMFLOAT3 right = m_camera->transform->GetRight();
	XMFLOAT3 forward = m_camera->transform->GetForward();
	const float speed = 50;

	
	if (m_keyboard->IsPressing('A')) {

		newPos += -right * speed * spf;
	}
	else if (m_keyboard->IsPressing('D')) {

		newPos += right * speed * spf;
	}
	if (m_keyboard->IsPressing('S')) {

		newPos += -forward * speed * spf;
	}
	else if (m_keyboard->IsPressing('W')) {

		newPos += forward * speed * spf;
	}
	static float angleX = 0;
	static float angleY = 0;
	static XMFLOAT2 prevMousePt;
	const float angleSpeed = 3.141592f * 0.2f;
	if (m_bPressingRMouse)
	{
		angleY += angleSpeed * spf * (m_scnMousePos.x - prevMousePt.x);
		angleX += angleSpeed * spf * (m_scnMousePos.y - prevMousePt.y);
	}
	prevMousePt.x = m_scnMousePos.x;
	prevMousePt.y = m_scnMousePos.y;
	const XMMATRIX rotMat = XMMatrixRotationX(angleX) * XMMatrixRotationY(angleY);
	m_camera->transform->SetTranslation(newPos);
	XMFLOAT3 f = MultiplyDir(FORWARD, rotMat);
	XMFLOAT3 u = MultiplyDir(UP, rotMat);
	m_camera->transform->SetRot(f, u);
	m_camera->Update();


	for (auto obj : m_vObj)
	{
		obj->Update();
	}

	for (auto obj : m_vObj)
	{
		obj->Render(m_dContext, m_camera->VMat(), m_camera->ProjMat(), m_camera->GetFrustum(), 0);
	}



}

void StartScene::WndProc(UINT MSG, WPARAM wparam, LPARAM lparam)
{
	switch (MSG)
	{
	case WM_COMMAND:
	{
		Timer* timer = (Timer*)lparam;

		Update(timer->Elapsed(), timer->SPF());
	}
		break;
	case WM_MOUSEMOVE:
	{
		POINTS p = MAKEPOINTS(lparam);
		m_scnMousePos.x = p.x;
		m_scnMousePos.y = p.y;
	}
		break;
	case WM_LBUTTONDOWN:
		m_bPressingLMouse = true;
		break;
	case WM_LBUTTONUP:
		m_bPressingLMouse = false;
		break;
	case WM_RBUTTONDOWN:
		m_bPressingRMouse = true;
		break;
	case WM_RBUTTONUP:
		m_bPressingRMouse = false;
		break;
	case WM_KEYDOWN:
		m_keyboard->Press(wparam);
		break;
	case WM_KEYUP:
		m_keyboard->Release(wparam);
		break;
	}
}
