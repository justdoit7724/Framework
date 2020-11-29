#include "stdafx.h"
#include "PlayScene.h"
#include "Keyboard.h"
#include "Timer.h"
#include "WindowDef.h"

using namespace DX;

XMFLOAT3 g_playerPos=XMFLOAT3(10,60,-30);
XMFLOAT3 g_playerForward = XMFLOAT3(0, 0, 1);
XMFLOAT3 g_playerUp= XMFLOAT3(0, 1, 0);

PlayScene::PlayScene(ID3D11Device* device, ID3D11DeviceContext* dContext, const wchar_t* key)
	:Scene(device, dContext, key)
{
	m_keyboard = new Keyboard();

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(D3D11_SAMPLER_DESC));
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	FLOAT black[4] = { 0,0,0,1 };
	sampDesc.BorderColor[0] = 0;
	sampDesc.BorderColor[1] = 0;
	sampDesc.BorderColor[2] = 0;
	sampDesc.BorderColor[3] = 1;
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	m_device->CreateSamplerState(&sampDesc, &m_dxSamp);
	
	m_dLight = new DX::DirectionalLight(
		device,
		0,
		XMFLOAT3(0.25, 0.25, 0.25),
		XMFLOAT3(0.8, 0.8, 0.8),
		XMFLOAT3(0.7, 0.7, 0.7),
		0.7f,
		DX::Normalize(XMFLOAT3(-1, -0.5f, -1))
	);
	m_dLight->Enable(true);

	m_camera = new DX::Camera("cam", DX::FRAME_KIND_PERSPECTIVE, NULL, NULL, 1.0f, 1000.0f, XM_PIDIV2, 1, false);
	m_camera->transform->SetTranslation(g_playerPos);
	m_camera->transform->SetRot(g_playerForward, g_playerUp);

	m_cbEye = new DX::Buffer(device, sizeof(XMFLOAT4));

	std::shared_ptr<DX::Mesh> pCubeMesh = std::make_shared<DX::CubeMesh>(m_device);

	DX::LoadTexture(m_device, m_dContext, "C:\\Users\\Jun\\source\\repos\\Framework\\Data\\Texture\\red_light.png", &m_dxRedSRV);
	DX::LoadTexture(m_device, m_dContext, "C:\\Users\\Jun\\source\\repos\\Framework\\Data\\Texture\\green_light.png", &m_dxGreenSRV);
	DX::LoadTexture(m_device, m_dContext, "C:\\Users\\Jun\\source\\repos\\Framework\\Data\\Texture\\blue_light.png", &m_dxBlueSRV);
	DX::LoadTexture(m_device, m_dContext, "C:\\Users\\Jun\\source\\repos\\Framework\\Data\\Texture\\white.png", &m_dxWhiteSRV);
	DX::LoadTexture(m_device, m_dContext, "C:\\Users\\Jun\\source\\repos\\Framework\\Data\\Texture\\default_normal.png", &m_dxNormSRV);


	m_dxRedBox = new DX::Object(m_device, m_dContext, "obj", pCubeMesh, nullptr, m_dxRedSRV, m_dxNormSRV);
	m_dxRedBox->transform->SetScale(40, 40, 20);
	m_dxRedBox->transform->SetTranslation(0, 20, 80);
	m_vObj.push_back(m_dxRedBox);
}

PlayScene::~PlayScene()
{
	m_dxRedSRV->Release();
	m_dxBlueSRV->Release();
	m_dxGreenSRV->Release();
	m_dxWhiteSRV->Release();
	m_dxNormSRV->Release();
	delete m_dLight;
	g_playerPos= m_camera->transform->GetPos();
	g_playerForward= m_camera->transform->GetForward();
	g_playerUp= m_camera->transform->GetUp();
	delete m_camera;
	delete m_cbEye;
	delete m_keyboard;
	m_dxSamp->Release();

	for (auto it = m_vObj.begin(); it != m_vObj.end(); ++it)
	{
		SAFEDELETE(*it);
	}

}

void PlayScene::Update(float elapsed, float spf)
{
	m_dLight->Apply(m_device, m_dContext);

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

	XMFLOAT3 eye = m_camera->transform->GetPos();

	m_cbEye->Write(m_dContext, &eye);
	m_dContext->PSSetConstantBuffers(SHADER_REG_CB_EYE, 1, m_cbEye->GetAddress());
	m_dContext->PSSetSamplers(SHADER_REG_SAMP_POINT, 1, &m_dxSamp);


	for (auto obj : m_vObj)
	{
		obj->Update();
	}

	for (auto obj : m_vObj)
	{
		obj->Render(m_dContext, m_camera->VMat(), m_camera->ProjMat(), m_camera->GetFrustum(), 0);
	}



}

void PlayScene::WM_LButtonDown()
{
}

void PlayScene::WM_RButtonDown()
{
	m_bPressingRMouse = true;
}

void PlayScene::WM_RButtonUp()
{
	m_bPressingRMouse = false;
}

void PlayScene::WM_KeyDown(WPARAM wparam)
{
	m_keyboard->Press(wparam);
}

void PlayScene::WM_KeyUp(WPARAM wparam)
{
	m_keyboard->Release(wparam);
}

void PlayScene::WM_MouseMove(LPARAM lparam)
{
	POINTS p = MAKEPOINTS(lparam);
	m_scnMousePos.x = p.x;
	m_scnMousePos.y = p.y;
}
