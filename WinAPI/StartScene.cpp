#include "stdafx.h"
#include "StartScene.h"
#include "Keyboard.h"

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
		XMFLOAT3(0.5, 0.5, 0.5),
		XMFLOAT3(0.8, 0.8, 0.8),
		XMFLOAT3(0.4, 0.4, 0.4),
		DX::Normalize(XMFLOAT3(-1, -0.5f, -1))
	);

	m_camera = new DX::Camera("cam", DX::FRAME_KIND_PERSPECTIVE, NULL, NULL, 1.0f, 1000.0f, XM_PIDIV2, 1, false);

	m_cbEye = new DX::Buffer(device, sizeof(XMFLOAT4));

	std::shared_ptr<DX::Mesh> pMesh = std::make_shared<DX::SphereMesh>(m_device, 5);

	ID3D11ShaderResourceView* diffSRV;
	ID3D11ShaderResourceView* normSRV;
	DX::LoadTexture(m_device, m_dContext, "C:\\Users\\Jun\\source\\repos\\Framework\\Data\\Texture\\sample.jpg", &diffSRV);
	DX::LoadTexture(m_device, m_dContext, "C:\\Users\\Jun\\source\\repos\\Framework\\Data\\Texture\\default_normal.png", &normSRV);


	auto obj = new DX::Object(m_device, m_dContext, "obj", pMesh, nullptr, diffSRV, normSRV);
	obj->transform->SetScale(20, 20, 20);
	obj->transform->SetTranslation(0,0,50);
	m_vObj.push_back(obj);

	obj = new DX::Object(m_device, m_dContext, "obj", pMesh, nullptr, diffSRV, normSRV);
	obj->transform->SetScale(20, 20, 20);
	obj->transform->SetTranslation(40,0,50);
	m_vObj.push_back(obj);

	obj = new DX::Object(m_device, m_dContext, "obj", pMesh, nullptr, diffSRV, normSRV);
	obj->transform->SetScale(20, 20, 20);
	obj->transform->SetTranslation(-40,0,50);
	m_vObj.push_back(obj);

	obj = new DX::Object(m_device, m_dContext, "obj", pMesh, nullptr, diffSRV, normSRV);
	obj->transform->SetScale(20, 20, 20);
	obj->transform->SetTranslation(0,40,50);
	m_vObj.push_back(obj);

	obj = new DX::Object(m_device, m_dContext, "obj", pMesh, nullptr, diffSRV, normSRV);
	obj->transform->SetScale(20, 20, 20);
	obj->transform->SetTranslation(0,-40,50);
	m_vObj.push_back(obj);

	obj = new DX::Object(m_device, m_dContext, "obj", pMesh, nullptr, diffSRV, normSRV);
	obj->transform->SetScale(20, 20, 20);
	obj->transform->SetTranslation(40, 40, 50);
	m_vObj.push_back(obj);

	obj = new DX::Object(m_device, m_dContext, "obj", pMesh, nullptr, diffSRV, normSRV);
	obj->transform->SetScale(20, 20, 20);
	obj->transform->SetTranslation(-40, 40, 50);
	m_vObj.push_back(obj);

	obj = new DX::Object(m_device, m_dContext, "obj", pMesh, nullptr, diffSRV, normSRV);
	obj->transform->SetScale(20, 20, 20);
	obj->transform->SetTranslation(-40, -40, 50);
	m_vObj.push_back(obj);

	obj = new DX::Object(m_device, m_dContext, "obj", pMesh, nullptr, diffSRV, normSRV);
	obj->transform->SetScale(20, 20, 20);
	obj->transform->SetTranslation(40, -40, 50);
	m_vObj.push_back(obj);

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

void StartScene::Mouse_LBtnDown()
{
	Scene::Mouse_LBtnDown();
}

void StartScene::Mouse_LBtnUp()
{
	Scene::Mouse_LBtnUp();
}

void StartScene::Mouse_RBtnDown()
{
	Scene::Mouse_RBtnDown();
}

void StartScene::Mouse_RBtnUp()
{
	Scene::Mouse_RBtnUp();
}

void StartScene::ReleaseKey(WPARAM wparam)
{
	Scene::ReleaseKey(wparam);


}

void StartScene::PressKey(WPARAM wparam)
{
	Scene::PressKey(wparam);


}

void StartScene::Mouse_UpdatePt(LPARAM lparam)
{
	Scene::Mouse_UpdatePt(lparam);
}

void StartScene::Mouse_Wheel(WPARAM wparam)
{
	Scene::Mouse_Wheel(wparam);
}
