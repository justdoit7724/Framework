#include "Scene.h"
#include "Game_info.h"
#include "Camera.h"
#include "Keyboard.h"
#include "TextureMgr.h"
#include "Camera.h"
#include "Timer.h"
#include "CustomFormat.h"
#include "UI.h"
#include "Object.h"
#include "Geometrics.h"
#include "Network.h"

#include "Hill.h"
#include "Sphere.h"
#include "Quad.h"
#include "Cube.h"
#include "Shader.h"
#include "Light.h"
#include "Texture2D.h"
#include "Buffer.h"

//delete 
Texture2D* copyTex;

Scene::Scene(IGraphic* graphic)
	:graphic(graphic)
{
	camera = new Camera(FRAME_KIND_PERSPECTIVE, SCREEN_WIDTH, SCREEN_HEIGHT, 0.1, 200, 1.1f, 1.0f, XMFLOAT3(0, 0, -100), FORWARD, RIGHT);
	canvas = new UICanvas(SCREEN_WIDTH, SCREEN_HEIGHT);
	Debugging::Instance()->EnableGrid(10, 50);

	dLight = new DirectionalLight(
		XMFLOAT3(0.25f, 0.25f, 0.25f),
		XMFLOAT3(0.7f, 0.7f, 0.7f),
		XMFLOAT3(0.8f, 0.8f, 0.8f),
		XMFLOAT3(0.707f, -0.707f,0));
	pLight = new PointLight(
		XMFLOAT3(0, 0.25f, 0),
		XMFLOAT3(0, 0.7f, 0),
		XMFLOAT3(0, 0.8f, 0),
		XMFLOAT3(0, 0, 0), 100, XMFLOAT3(0.25f, 0.025f, 0.0025f)
	);

	TextureMgr::Instance()->Load("sample.jpg");
	TextureMgr::Instance()->Load("white.png");
	TextureMgr::Instance()->Load("black.png");
	TextureMgr::Instance()->Load("marine_s.png");
	TextureMgr::Instance()->Load("heightmap3.jpg");
	TextureMgr::Instance()->Load("woodbox.jpg");

	Object* wall = new Object(new Cube(), XMFLOAT3(1, 1, 1), XMFLOAT3(1, 1, 1), XMFLOAT3(1, 1, 1), 4, XMFLOAT3(1, 1, 1), "woodbox.jpg");
	wall->GetTransform()->SetScale(40, 40, 4);
	D3D11_DEPTH_STENCIL_DESC maskDesc;
	maskDesc.DepthEnable = true;
	maskDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	maskDesc.DepthFunc = D3D11_COMPARISON_LESS;
	maskDesc.StencilEnable = true;
	maskDesc.StencilReadMask = 0xff;
	maskDesc.StencilWriteMask = 0xff;
	maskDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	maskDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_REPLACE;
	maskDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	maskDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	maskDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	maskDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	maskDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	maskDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	wall->GetDepthStencilState()->SetRefValue(255);
	wall->GetDepthStencilState()->Modify(&maskDesc);

	objs.push_back(wall);

	D3D11_TEXTURE2D_DESC desc;
	desc.ArraySize = 1;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.Format = DXGI_FORMAT_X24_TYPELESS_G8_UINT;
	desc.Width = SCREEN_WIDTH;
	desc.Height = SCREEN_HEIGHT;
	desc.MipLevels = 1;
	desc.MiscFlags = 0;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	copyTex = new Texture2D(&desc);
}

Scene::~Scene()
{
	delete camera;
	delete canvas;
	delete dLight;
}


ID3D11ShaderResourceView* depthComplexSrv;
void Scene::Update()
{
	Timer::Update();
	camera->Update(Timer::SPF());

	/*if (Keyboard::IsPressing("F"))
	{
		dLightDir = dLightDir*XMMatrixRotationZ(0.01f);
	}
	else if (Keyboard::IsPressing("H"))
	{
		dLightDir = dLightDir * XMMatrixRotationZ(-0.01f);

	}
	if (Keyboard::IsPressing("T"))
	{
		dLightDir = dLightDir * XMMatrixRotationX(0.01f);
	}
	else if (Keyboard::IsPressing("G"))
	{
		dLightDir = dLightDir * XMMatrixRotationX(-0.01f);
	}
	dLight->SetDir(dLightDir);*/

	if (pLight)
	{
		XMFLOAT3 pt = XMFLOAT3(
			cos(Timer::Elapsed()*0.5f) * 40,
			35,
			sin(Timer::Elapsed()*0.5f) * 60);
		pLight->SetPos(pt);
		Debugging::Instance()->Mark(0, pt, 1.5f, Colors::Green);
	}

	
	

	for (auto obj : objs)
	{
		obj->Update(camera, DirectionalLight::Data(), PointLight::Data(), SpotLight::Data(), XMMatrixIdentity());
	}

}

void Scene::Render()
{
	Debugging::Instance()->Render(camera);

	for (auto obj : objs)
	{
		obj->Render();
	}

	DX_DContext->CopyResource(copyTex->Get(), graphic->DepthStencilBuffer());

	static bool done = false;
	if (!done)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_X24_TYPELESS_G8_UINT;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Texture2D.MostDetailedMip = 0;
		r_assert(DX_Device->CreateShaderResourceView(copyTex->Get(), &srvDesc, &depthComplexSrv));

		canvas->AddDC("DC", XMFLOAT2(0, 0), 300, 300, 0, depthComplexSrv);
	}

	canvas->Update(Timer::SPF());
	canvas->Render();

}
