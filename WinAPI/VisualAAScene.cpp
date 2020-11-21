#include "stdafx.h"
#include "VisualAAScene.h"
#include "Keyboard.h"
#include "Timer.h"
#include "WindowDef.h"

using namespace DX;

VisualAAScene::VisualAAScene(ID3D11Device* device, ID3D11DeviceContext* dContext, const wchar_t* key)
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

	m_camera = new DX::Camera("cam", DX::FRAME_KIND_ORTHOGONAL, 600, 400, 1.0f, 100, NULL, NULL, true);
	m_camera->transform->SetTranslation(0, 0, -50);
	m_camera->Update();

	m_cbPrimitive = new DX::Buffer(device, sizeof(XMFLOAT4)*8);

	std::shared_ptr<DX::Mesh> pQuadMesh = std::make_shared<DX::QuadMesh>(m_device);


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
	redSRV->Release();
	blueSRV->Release();
	greenSRV->Release();
	whiteSRV->Release();
	normSRV->Release();

	m_dxSampling = new DX::SamplingDraw(device, dContext);
	m_vObj.push_back(m_dxSampling);

	m_dxQuad1 = new DX::UnlitObj(m_device, m_dContext, "quad1", pQuadMesh, nullptr, XMVectorSet(1, 0, 0, 1),true);
	m_dxQuad1->transform->SetScale(240, 240, 1);
	m_dxQuad1->transform->SetTranslation(-110, 15, 40.0);
	m_dxQuad2 = new DX::UnlitObj(m_device, m_dContext, "quad2", pQuadMesh, nullptr, XMVectorSet(0, 1, 0, 1),true);
	m_dxQuad2->transform->SetScale(340, 90, 1);
	m_dxQuad2->transform->SetTranslation(110, -10, -20);
	D3D11_RASTERIZER_DESC rsDesc;
	rsDesc.CullMode = D3D11_CULL_BACK;
	rsDesc.FillMode = D3D11_FILL_WIREFRAME;
	rsDesc.FrontCounterClockwise = FALSE;
	rsDesc.DepthBias = 0;
	rsDesc.DepthBiasClamp = 0;
	rsDesc.SlopeScaledDepthBias = 0;
	rsDesc.DepthClipEnable = TRUE;
	rsDesc.ScissorEnable = FALSE;
	rsDesc.MultisampleEnable = FALSE;
	rsDesc.AntialiasedLineEnable = FALSE;
	delete m_dxQuad1->rsState;
	delete m_dxQuad2->rsState;
	m_dxQuad1->rsState = new DX::RasterizerState(device, &rsDesc);
	m_dxQuad2->rsState = new DX::RasterizerState(device, &rsDesc);
	m_vObj.push_back(m_dxQuad1);
	m_vObj.push_back(m_dxQuad2);
	
}

VisualAAScene::~VisualAAScene()
{
	delete m_camera;
	delete m_cbPrimitive;
	delete m_keyboard;

	for (auto it = m_vObj.begin(); it != m_vObj.end(); ++it)
	{
		SAFEDELETE(*it)
	}
}

void VisualAAScene::Update(float elapsed, float spf)
{
	XMFLOAT3 upDir1 = XMFLOAT3(
		-sin(elapsed / 21),
		cos(elapsed / 21),
		0
	);
	XMFLOAT3 upDir2 = XMFLOAT3(
		-sin(elapsed / 17),
		cos(elapsed / 17),
		0
	);
	m_dxQuad1->transform->SetRot(-FORWARD, upDir1);
	m_dxQuad2->transform->SetRot(-FORWARD, upDir2);
	
	XMFLOAT4 rgVertice[] =
	{
		XMFLOAT4(-0.5, -0.5, 0,1),
		XMFLOAT4(-0.5, 0.5, 0,1),
		XMFLOAT4(0.5, 0.5, 0,1),
		XMFLOAT4(0.5, -0.5, 0,1),
		XMFLOAT4(-0.5, -0.5, 0,1),
		XMFLOAT4(-0.5, 0.5, 0,1),
		XMFLOAT4(0.5, 0.5, 0,1),
		XMFLOAT4(0.5, -0.5, 0,1)
	};
	XMMATRIX mvp1 = m_dxQuad1->transform->WorldMatrix() * m_camera->VMat() * m_camera->ProjMat();
	XMMATRIX mvp2 = m_dxQuad2->transform->WorldMatrix() * m_camera->VMat() * m_camera->ProjMat();
	for (int i = 0; i < 4; ++i)
	{
		rgVertice[i] = Multiply(rgVertice[i], mvp1);
		//rgVertice[i] /= rgVertice[i].w;
	}
	for (int i = 4; i < 8; ++i)
	{
		rgVertice[i] = Multiply(rgVertice[i], mvp2);
		//rgVertice[i] /= rgVertice[i].w;
	}
	m_cbPrimitive->Write(m_dContext, rgVertice);
	m_dContext->PSSetConstantBuffers(9, 1, m_cbPrimitive->GetAddress());

	for (auto obj : m_vObj)
	{
		obj->Update();
	}


	XMMATRIX matV= m_camera->VMat();
	XMMATRIX matP=m_camera->ProjMat();
	DX::Frustum frustum = m_camera->GetFrustum();

	for (auto obj : m_vObj)
	{
		obj->Render(m_dContext, matV, matP, frustum, 0);
	}
}

void VisualAAScene::WM_Resize(int width, int height)
{
	m_iWidth = width;
	m_iHeight = height;
	m_camera->SetFrame(FRAME_KIND::FRAME_KIND_ORTHOGONAL, XMFLOAT2(width, height), 0.01, 100, NULL, NULL);

}

void VisualAAScene::SetResolution(int res)
{
	m_dxSampling->SetResolution(res);
}

void VisualAAScene::WM_RButtonDown()
{
	m_bPressingRMouse = true;
}

void VisualAAScene::WM_RButtonUp()
{
	m_bPressingRMouse = false;
}

void VisualAAScene::WM_KeyDown(WPARAM wparam)
{
	m_keyboard->Press(wparam);
}

void VisualAAScene::WM_KeyUp(WPARAM wparam)
{
	m_keyboard->Release(wparam);
}

void VisualAAScene::WM_MouseMove(LPARAM lparam)
{
	POINTS p = MAKEPOINTS(lparam);
	m_scnMousePos.x = p.x;
	m_scnMousePos.y = p.y;
}
