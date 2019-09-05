#include "Scene.h"
#include "Game_info.h"
#include "Camera.h"
#include "Keyboard.h"
#include "TextureMgr.h"
#include "Network.h"
#include "Camera.h"
#include "Timer.h"
#include "CustomFormat.h"
#include "UI.h"
#include "Object.h"
#include "Geometrics.h"

#include "Hill.h"
#include "Sphere.h"
#include "Quad.h"
#include "Cube.h"
#include "Shader.h"
#include "Light.h"

Scene::Scene(IGraphic* graphic)
{
	ID3D11Device* device = graphic->Device();

	camera = new Camera(FRAME_KIND_PERSPECTIVE, SCREEN_WIDTH, SCREEN_HEIGHT ,0.1,200,1.1f, 1.0f, XMFLOAT3(0,0,-100), FORWARD, RIGHT);

	//canvas = new UICanvas(device, SCREEN_WIDTH, SCREEN_HEIGHT);

	Debugging::EnableGrid(10,50);

	UINT spriteX=8;
	TextureMgr::Instance()->Load(graphic, "grass.jpg");
	TextureMgr::Instance()->Load(graphic, "marine_s.png", spriteX);

	ID3D11ShaderResourceView* srv=nullptr;
	TextureMgr::Instance()->Get("marine_s.png", &srv, &spriteX);
	
	//canvas->Add(device, "Test", XMFLOAT2(0, 0), 200, 200, 1, srv, spriteX, 12);

	Sphere* tempQuad = new Sphere(device,3);

	int objCountX = 6;
	int objCountY = 4;
	for (int z = 0; z < objCountY; ++z)
	{
		for (int x = 0; x < objCountX; ++x)
		{
			Object* newObj = new Object(
				graphic,
				tempQuad,
				XMFLOAT3(0.8f, 0.8f, 0.8f), XMFLOAT3(0.15f, 0.15f, 0.15f), XMFLOAT3(0.9f, 0.9f, 0.9f), 8.0f, XMFLOAT3(1, 1, 1),
				"grass.jpg"
			);
			newObj->GetTransform()->SetScale(XMFLOAT3(2.0f, 2.0f, 2.0f));
			//newObj->GetTransform()->SetRot(UP, -FORWARD);
			newObj->GetTransform()->SetTranslation(x*10,0,z*10);

			objs.push_back(newObj);

		}
	}


	/*TextureMgr::Instance()->Load(graphic, "heightmap3.jpg");
	hill = new Hill(device, graphic->DContext(), 200, 200, XMFLOAT2(0,30), TextureMgr::Instance()->Get("heightmap3.jpg"));
	sphere = new Sphere(device, 3);
	temp = new Object(graphic, sphere, XMFLOAT3(1, 1, 1), XMFLOAT3(1, 1, 1), XMFLOAT3(1, 1, 1), 8, XMFLOAT3(1, 1, 1), "grass.jpg");
	shader = new VPShader(device, L"StandardVS.cso", L"StandardPS.cso", std_ILayouts, ARRAYSIZE(std_ILayouts));
	material = new ShaderMaterial(XMFLOAT3(1, 1, 1), XMFLOAT3(1, 1, 1), XMFLOAT3(1, 1, 1), 8, XMFLOAT3(1, 1, 1));
	cb_vs_property = new ConstantBuffer<VS_Property>(device);
	cb_ps_dLights = new ConstantBuffer<SHADER_DIRECTIONAL_LIGHT>(device);
	cb_ps_pLights = new ConstantBuffer<SHADER_POINT_LIGHT>(device);
	cb_ps_sLights = new ConstantBuffer<SHADER_SPOT_LIGHT>(device);
	cb_ps_eyePos = new ConstantBuffer<XMFLOAT3>(device);
	cb_ps_material = new ConstantBuffer<ShaderMaterial>(device);
	bodySrv = TextureMgr::Instance()->Get("grass.jpg");
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	r_assert(
		device->CreateSamplerState(&samplerDesc, &samplerState)
	);*/

	dLight = new DirectionalLight(
		XMFLOAT3(0.15f, 0.15f, 0.15f),
		XMFLOAT3(0.75f, 0.75f, 0.75f),
		XMFLOAT3(0.9f, 0.9f, 0.9f),
		FORWARD);
}

Scene::~Scene()
{
}

//static XMMATRIX
void Scene::Update()
{
	Timer::Update();
	camera->Update(Timer::SPF());
	//canvas->Update(Timer::SPF());

	//Debugging::Draw("Camera1 Pos", camera->Pos(), 10, 10);

}

void Scene::Render(IGraphic* graphic)
{
	Debugging::Render(camera, graphic);

	for (auto obj : objs)
	{
		obj->Render(graphic, camera, DirectionalLight::Data(), PointLight::Data(), SpotLight::Data(), XMMatrixIdentity());
	}

	/*ID3D11DeviceContext* dContext = graphic->DContext();

	shader->SetPipline(dContext);

	cb_vs_property->VSSetData(dContext, &VS_Property(camera->VPMat(2)));

	cb_ps_dLights->PSSetData(dContext, DirectionalLight::Data(),0);
	cb_ps_pLights->PSSetData(dContext, PointLight::Data(),1);
	cb_ps_sLights->PSSetData(dContext, SpotLight::Data(),2);
	cb_ps_eyePos->PSSetData(dContext, &camera->Pos(), 3);
	cb_ps_material->PSSetData(dContext, &ShaderMaterial(XMFLOAT3(1, 1, 1), XMFLOAT3(1, 1, 1), XMFLOAT3(1, 1, 1), 8, XMFLOAT3(1, 1, 1)),4);
	dContext->PSSetSamplers(0, 1, &samplerState);
	dContext->PSSetShaderResources(0, 1, &bodySrv);

	graphic->SetDepthStencilState();
	graphic->SetRasterizerState();

	sphere->Render(dContext);*/

	//temp->Render(graphic, camera, DirectionalLight::Data(), PointLight::Data(), SpotLight::Data(),XMMatrixIdentity());

	//canvas->Render(graphic);

}
