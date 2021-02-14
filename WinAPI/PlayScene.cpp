#include "stdafx.h"
#include "PlayScene.h"
#include "Keyboard.h"
#include "Timer.h"
#include "WindowDef.h"

using namespace DX;

PlayScene::PlayScene(DX::Graphic* graphic, const wchar_t* key)
	:Scene(graphic, key)
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
	graphic->Device()->CreateSamplerState(&sampDesc, &m_dxSamp);
	m_dxGraphic->DContext()->PSSetSamplers(SHADER_REG_SAMP_POINT, 1, &m_dxSamp);
	
	m_dLight = new DX::DirectionalLight(
		graphic->Device(),
		0,
		XMFLOAT3(0.4, 0.4, 0.4),
		XMFLOAT3(0.8, 0.8, 0.8),
		XMFLOAT3(0.7, 0.7, 0.7),
		1.7f,
		DX::Normalize(XMFLOAT3(1, -0.5f, 1))
	);
	m_dLight->Enable(true);

	m_camera = new DX::Camera("cam", DX::FRAME_KIND_PERSPECTIVE, NULL, NULL, 0.001f, 500, XM_PIDIV2, 1, false);
	m_camera->transform->SetTranslation(0,0,0);
	m_camera->transform->SetRot(FORWARD);
	m_camera->Update();


	m_cbEye = new DX::Buffer(graphic->Device(), sizeof(XMFLOAT4));

	std::vector<DX::Mesh*> vMesh;
	DX::SkinnedData* skinnedData = new DX::SkinnedData();
	DX::LoadOBJ(graphic->Device(), skinnedData, "C:\\Users\\Jun\\Downloads\\model(bot)\\56-sphere-bot-basic\\Sphere-Bot Basic", "Armature_001-(COLLADA_3 (COLLAborative Design Activity)).dae");

	std::shared_ptr<DX::Mesh> pObjMesh(vMesh[0]);

	DX::LoadTexture(graphic->Device(), graphic->DContext(), __FILE__"\\..\\..\\Data\\Texture\\red_light.png", &m_dxRedSRV);
	DX::LoadTexture(graphic->Device(), graphic->DContext(), __FILE__"\\..\\..\\Data\\Texture\\green_light.png", &m_dxGreenSRV);
	DX::LoadTexture(graphic->Device(), graphic->DContext(), __FILE__"\\..\\..\\Data\\Texture\\blue_light.png", &m_dxBlueSRV);
	DX::LoadTexture(graphic->Device(), graphic->DContext(), __FILE__"\\..\\..\\Data\\Texture\\white.png", &m_dxWhiteSRV);
	DX::LoadTexture(graphic->Device(), graphic->DContext(), __FILE__"\\..\\..\\Data\\Texture\\default_normal.png", &m_dxNormSRV);


	DX::Object* dxObj;
	dxObj = new DX::Object(graphic->Device(), graphic->DContext(), "obj", pObjMesh, nullptr, m_dxRedSRV, m_dxNormSRV);
	dxObj->transform->SetScale(1, 1, 1);
	m_vObj.push_back(dxObj);
}

PlayScene::~PlayScene()
{
	m_dxRedSRV->Release();
	m_dxBlueSRV->Release();
	m_dxGreenSRV->Release();
	m_dxWhiteSRV->Release();
	m_dxNormSRV->Release();
	SAFEDELETE(m_dLight);
	SAFEDELETE(m_camera);
	SAFEDELETE(m_cbEye);
	SAFEDELETE(m_keyboard);
	m_dxSamp->Release();

	for (auto it = m_vObj.begin(); it != m_vObj.end(); ++it)
	{
		SAFEDELETE(*it);
	}
}

void PlayScene::Update(float elapsed, float spf)
{
	m_dLight->Apply(m_dxGraphic->Device(), m_dxGraphic->DContext());

	XMFLOAT3 newPos = m_camera->transform->GetPos();
	XMFLOAT3 right = m_camera->transform->GetRight();
	XMFLOAT3 forward = m_camera->transform->GetForward();
	const float speed = 75;

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

	m_cbEye->Write(m_dxGraphic->DContext(), &eye);
	m_dxGraphic->DContext()->PSSetConstantBuffers(SHADER_REG_CB_EYE, 1, m_cbEye->GetAddress());

	XMMATRIX v = m_camera->VMat();
	XMMATRIX p = m_camera->ProjMat();

	for (auto it = m_vObj.begin(); it != m_vObj.end(); ++it)
	{
		(*it)->Update();
		(*it)->Render(m_dxGraphic->DContext(), v, p, nullptr);
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

void PlayScene::WM_MouseWheel(WPARAM wparam)
{
}
