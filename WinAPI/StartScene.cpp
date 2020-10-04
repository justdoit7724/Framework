#include "stdafx.h"
#include "StartScene.h"

StartScene::StartScene(ID3D11Device* device, ID3D11DeviceContext* dContext, const wchar_t* key)
	:Scene(device, dContext, key)
{
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
}

void StartScene::Update(float elapsed, float spf)
{
	XMFLOAT3 eye = m_camera->transform->GetPos();

	m_cbEye->Write(m_dContext, &eye);
	m_dContext->PSSetConstantBuffers(SHADER_REG_CB_EYE, 1, m_cbEye->GetAddress());
	m_dLight->Apply(m_device,m_dContext);
	
	m_camera->Move(XMFLOAT2(0, 0), spf);

	for (auto obj : m_vObj)
	{
		obj->Update();
	}

	XMMATRIX vp = m_camera->VMat() * m_camera->ProjMat();
	for (auto obj : m_vObj)
	{
		obj->Render(m_dContext, vp, m_camera->GetFrustum(), 0);
	}
}
