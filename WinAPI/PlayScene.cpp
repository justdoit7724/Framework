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
		XMFLOAT3(0.25, 0.25, 0.25),
		XMFLOAT3(0.8, 0.8, 0.8),
		XMFLOAT3(0.7, 0.7, 0.7),
		0.7f,
		DX::Normalize(XMFLOAT3(-1, -0.5f, -1))
	);
	m_dLight->Enable(false);
	m_pLight = new DX::PointLight(
		device,
		XMFLOAT3(0.25, 0.25, 0.25),
		XMFLOAT3(0.8, 0.8, 0.8),
		XMFLOAT3(0.5, 0.5, 0.5),
		300,
		XMFLOAT3(0.01f, 0.01f, 0.01f),
		XMFLOAT3(0, 30, 0)
	);
	m_pLight->SetPos(XMFLOAT3(15, 30, 35));
	m_pLight->Enable(false);
	m_sLight = new DX::SpotLight(
		device,
		XMFLOAT3(0.25, 0.25, 0.25),
		XMFLOAT3(0.8, 0.8, 0.8),
		XMFLOAT3(0.7, 0.7, 0.7),
		60, 2, 1000, XM_PIDIV4,
		XMFLOAT3(2, 2, 2),
		XMFLOAT3(0, 20, 0),
		FORWARD
	);
	m_sLight->SetPos(XMFLOAT3(-15, 30, -40));
	m_sLight->Enable(false);

	m_camera = new DX::Camera("cam", DX::FRAME_KIND_PERSPECTIVE, NULL, NULL, 1.0f, 1000.0f, XM_PIDIV2, 1, false);
	m_camera->transform->SetTranslation(g_playerPos);
	m_camera->transform->SetRot(g_playerForward, g_playerUp);

	m_cbEye = new DX::Buffer(device, sizeof(XMFLOAT4));

	std::shared_ptr<DX::Mesh> pSphereMesh = std::make_shared<DX::SphereMesh>(m_device, 6);
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
	m_dxBlueBox->transform->SetTranslation(0, 20, 80);
	m_vObj.push_back(m_dxBlueBox);

	m_dxBlueBox = new DX::Object(m_device, m_dContext, "obj", pSphereMesh, nullptr, greenSRV, normSRV);
	m_dxBlueBox->transform->SetScale(20, 20, 20);
	m_dxBlueBox->transform->SetTranslation(40, 40, 50);
	m_vObj.push_back(m_dxBlueBox);

	m_dxBlueBox = new DX::Object(m_device, m_dContext, "obj", pCubeMesh, nullptr, blueSRV, normSRV);
	m_dxBlueBox->transform->SetScale(25, 35, 20);
	m_dxBlueBox->transform->SetTranslation(-10, 25, 0);
	m_vObj.push_back(m_dxBlueBox);

	m_dxPLightBlub = new DX::UnlitObj(m_device, m_dContext, "plightBulb", pSphereMesh, nullptr, XMVectorSet(1, 1, 1, 1));
	m_dxPLightBlub->transform->SetTranslation(m_pLight->GetPos());
	m_dxPLightBlub->transform->SetScale(4, 4, 4);
	m_dxPLightBlub->SetEnabled(false);
	m_vObj.push_back(m_dxPLightBlub);
	m_dxSLightBlub = new DX::UnlitObj(m_device, m_dContext, "slightBulb", pSphereMesh, nullptr, XMVectorSet(1, 1, 1, 1));
	m_dxSLightBlub->transform->SetTranslation(m_sLight->GetPos());
	m_dxSLightBlub->transform->SetScale(4, 4, 4);
	m_dxSLightBlub->SetEnabled(false);
	m_vObj.push_back(m_dxSLightBlub);

	DX::Object* dxFloor = new DX::Object(m_device, m_dContext, "obj", pFloorMesh, nullptr, whiteSRV, normSRV);
	dxFloor->transform->SetScale(300, 300, 1);
	dxFloor->transform->SetRot(UP);
	m_vObj.push_back(dxFloor);

	EnableDLight(true);
	EnablePLight(true);
	EnableSLight(true);
}

PlayScene::~PlayScene()
{
	delete m_dLight;
	g_playerPos= m_camera->transform->GetPos();
	g_playerForward= m_camera->transform->GetForward();
	g_playerUp= m_camera->transform->GetUp();
	delete m_camera;
	delete m_cbEye;
	delete m_keyboard;
	m_dxSamp->Release();
}

void PlayScene::Update(float elapsed, float spf)
{
	/*const float lightMoveRad = 50;
	const float lightMoveSpeed = 10;
	const float mElapsed = elapsed / 5;
	XMFLOAT3 pLightPos = (RIGHT * cos(mElapsed) + FORWARD * sin(mElapsed)) * lightMoveRad + UP * 25;
	XMFLOAT3 sLightPos = (RIGHT * cos(mElapsed * 1.3) + FORWARD * sin(mElapsed * 1.3)) * lightMoveRad + UP * 20;
	m_pLight->SetPos(pLightPos);
	m_dxPLightBlub->transform->SetTranslation(pLightPos);
	m_sLight->SetPos(sLightPos);
	m_dxSLightBlub->transform->SetTranslation(sLightPos);*/

	m_dLight->Apply(m_device, m_dContext);
	m_pLight->Apply(m_device, m_dContext);
	m_sLight->Apply(m_device, m_dContext);

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

void PlayScene::EnableDLight(bool b)
{
	m_dLight->Enable(b);
}

void PlayScene::EnablePLight(bool b)
{
	m_pLight->Enable(b);
	m_dxPLightBlub->SetEnabled(b);
}

void PlayScene::EnableSLight(bool b)
{
	m_sLight->Enable(b);
	m_dxSLightBlub->SetEnabled(b);
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
