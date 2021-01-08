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

	m_camera = new DX::Camera("cam", DX::FRAME_KIND_PERSPECTIVE, NULL, NULL, 0.001f, 500.0, XM_PIDIV2, 1, false);
	//m_camera = new DX::Camera("cam", DX::FRAME_KIND_ORTHOGONAL, 600, 600, 0.001f, 100.0, NULL, NULL, false);
	m_camera->transform->SetTranslation(0,0,0);
	m_camera->transform->SetRot(FORWARD);
	m_camera->Update();

	auto mat = m_camera->ProjMat();
	auto result = DX::Multiply(XMFLOAT4(300, 0, 10, 1), mat);


	m_cbEye = new DX::Buffer(graphic->Device(), sizeof(XMFLOAT4));

	m_dxDepthPeeling = new DX::DepthPeeling(graphic);

	std::shared_ptr<DX::Mesh> pCubeMesh = std::make_shared<DX::CubeMesh>(graphic->Device());
	std::shared_ptr<DX::Mesh> pSphereMesh = std::make_shared<DX::SphereMesh>(graphic->Device(), 4);

	DX::LoadTexture(graphic->Device(), graphic->DContext(), "C:\\Users\\Jun\\source\\repos\\Framework\\Data\\Texture\\red_light.png", &m_dxRedSRV);
	DX::LoadTexture(graphic->Device(), graphic->DContext(), "C:\\Users\\Jun\\source\\repos\\Framework\\Data\\Texture\\green_light.png", &m_dxGreenSRV);
	DX::LoadTexture(graphic->Device(), graphic->DContext(), "C:\\Users\\Jun\\source\\repos\\Framework\\Data\\Texture\\blue_light.png", &m_dxBlueSRV);
	DX::LoadTexture(graphic->Device(), graphic->DContext(), "C:\\Users\\Jun\\source\\repos\\Framework\\Data\\Texture\\white.png", &m_dxWhiteSRV);
	DX::LoadTexture(graphic->Device(), graphic->DContext(), "C:\\Users\\Jun\\source\\repos\\Framework\\Data\\Texture\\default_normal.png", &m_dxNormSRV);

	D3D11_DEPTH_STENCIL_DESC dsDesc;
	ZeroMemory(&dsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dsDesc.DepthEnable = FALSE;
	dsDesc.StencilEnable = FALSE;
	D3D11_BLEND_DESC blendDesc;
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
	m_dxRedBox = new DX::Object(graphic->Device(), graphic->DContext(), "obj", pCubeMesh, nullptr, m_dxRedSRV, m_dxNormSRV);
	m_dxRedBox->transform->SetScale(100, 100, 20);
	m_dxRedBox->transform->SetTranslation(0, 0, 80);
	m_dxRedBox->blendState->Modify(graphic->Device(), &blendDesc);
	m_dxRedBox->dsState->Modify(graphic->Device(), &dsDesc);

	m_dxGreenBox = new DX::Object(graphic->Device(), graphic->DContext(), "obj", pCubeMesh, nullptr, m_dxGreenSRV, m_dxNormSRV);
	m_dxGreenBox->transform->SetScale(180, 180, 10);
	m_dxGreenBox->transform->Rotate(-FORWARD, XM_PI / 18);
	m_dxGreenBox->transform->SetTranslation(0, 0, 160);
	m_dxGreenBox->blendState->Modify(graphic->Device(), &blendDesc);
	m_dxGreenBox->dsState->Modify(graphic->Device(), &dsDesc);

	m_vObj.push_back(m_dxGreenBox);
	m_vObj.push_back(m_dxRedBox);

	/*m_dxBlueBox = new DX::Object(graphic->Device(), graphic->DContext(), "obj", pCubeMesh, nullptr, m_dxBlueSRV, m_dxNormSRV);
	m_dxBlueBox->transform->SetScale(50, 50, 10);
	m_dxBlueBox->transform->Rotate(-FORWARD, XM_PI / 9);
	m_dxBlueBox->transform->SetTranslation(0, 0, 90);
	m_vObj.push_back(m_dxBlueBox);*/

	
	
	m_dxCanvas = new DX::UICanvas(graphic->DContext(), 600, 600);
	m_dxTestUI = new DX::UI(graphic->Device(), graphic->DContext(), XMFLOAT2(150, 150), XMFLOAT2(300, 300), 0, nullptr);

	//m_dxCanvas->Add(m_dxTestUI);

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
	SAFEDELETE(m_dxDepthPeeling);
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

	m_cbEye->Write(m_dxGraphic->DContext(), &eye);
	m_dxGraphic->DContext()->PSSetConstantBuffers(SHADER_REG_CB_EYE, 1, m_cbEye->GetAddress());

	std::vector<Light*> vLight;
	vLight.push_back(m_dLight);
	m_dxDepthPeeling->Run(m_camera, vLight, m_vObj, 2);
	m_dxDepthPeeling->Render(m_dxGraphic->DContext());
	
	
	/*for (auto obj : m_vObj)
	{
		obj->Update();
	}

	for (auto obj : m_vObj)
	{
		obj->Render(m_dxGraphic->DContext(), m_camera->VMat(), m_camera->ProjMat(), m_camera->GetFrustum());
	}*/
	
	m_dxCanvas->Update(m_scnMousePos);

	//m_dxGraphic->BindView();
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

#include <opencv2/opencv.hpp>
#pragma comment(lib,"C:/IMS/Library/opencv/x64/vc15/lib/opencv_world430d.lib")

void PlayScene::MakeDepthFile(ID3D11DepthStencilView* dsv, std::string strFileName)
{
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ID3D11Texture2D* depthTex;
	ID3D11Texture2D* stageTex;
	D3D11_TEXTURE2D_DESC stageTexDesc;

	ID3D11Resource* tmpResource;
	dsv->GetResource(&tmpResource);
	depthTex = (ID3D11Texture2D*)tmpResource;

	depthTex->GetDesc(&stageTexDesc);

	stageTexDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	stageTexDesc.BindFlags = 0;
	stageTexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	stageTexDesc.Usage = D3D11_USAGE_STAGING;
	m_dxGraphic->Device()->CreateTexture2D(&stageTexDesc, nullptr, &stageTex);
	
	m_dxGraphic->DContext()->CopyResource(stageTex, depthTex);
	
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	m_dxGraphic->DContext()->Map(stageTex, 0, D3D11_MAP_READ, 0, &mappedResource);
	int nSize = mappedResource.RowPitch * stageTexDesc.Height;
	unsigned char*const pTest = (unsigned char*)malloc(nSize);
	ZeroMemory(pTest, nSize);
	memcpy(pTest, mappedResource.pData, nSize);
	m_dxGraphic->DContext()->Unmap(stageTex, 0);

	std::ofstream outdata;
	std::string strFullName = "C:\\Users\\Jun\\Desktop\\depth_result\\" + strFileName + ".csv";
	outdata.open(strFullName.c_str());
	if (!outdata.is_open()) return;

	outdata << "WIDTH = " << stageTexDesc.Width << std::endl;
	outdata << "HEIGHT = " << stageTexDesc.Height << std::endl;
	for (int y = 0; y < stageTexDesc.Height; ++y)
	{
		for (int x = 0; x < stageTexDesc.Width; ++x)
		{
			int index = x + y * stageTexDesc.Width;

			int value = *(int*)(pTest + size_t(mappedResource.RowPitch) * y + x * sizeof(int));

			outdata << value << ",";
		}
		outdata << std::endl;
	}

	outdata.close();

	stageTex->Release();

	exit(-1);
}
