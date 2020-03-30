#include "stdafx.h"
#include "ShaderReg.h"
#include "TestScene.h"
#include "TextureMgr.h"
#include "stdafx.h"
#include "Object.h"
#include "SphereMesh.h"
#include "Transform.h"
#include "Buffer.h"
#include "Camera.h"
#include "Shader.h"
#include "CameraMgr.h"
#include "ShaderFormat.h"
#include "Light.h"
#include "Algorithm.h"
#include "UI.h"
#include "Debugging.h"
#include "SphereCollider.h"
#include "CubeCollider.h"

UIButton* btn; Object* newObj;
TestScene::TestScene()
{
	canvas = new UICanvas();

	dLight = new DirectionalLight(
		XMFLOAT3(0.4, 0.4, 0.4),
		XMFLOAT3(0.7, 0.7, 0.7),
		XMFLOAT3(0.9, 0.9, 0.9),
		Normalize(XMFLOAT3(1, -0.5, 1)));

	TextureMgr::Instance()->Load("green", "Data\\Texture\\green_light.png");
	TextureMgr::Instance()->Load("red", "Data\\Texture\\red_light.png");
	TextureMgr::Instance()->Load("blue", "Data\\Texture\\blue_light.png");
	
	std::shared_ptr<Mesh> sphere = std::make_shared<SphereMesh>(4);
	std::shared_ptr<Collider> collider = std::make_shared<CubeCollider>();
	newObj = new Object("Obj", sphere, collider,
		TextureMgr::Instance()->Get("green"));

	newObj->transform->SetScale(XMFLOAT3(10, 10, 10));
	Debugging::Instance()->Visualize(newObj->collider.get());

	btn = new UIButton(0, nullptr, XMFLOAT2(100, 100), XMFLOAT2(400, 300), TextureMgr::Instance()->Get("green"), TextureMgr::Instance()->Get("blue"), TextureMgr::Instance()->Get("red"));
	//Debugging::Instance()->Visualize(btn);

	cbEye = new Buffer(sizeof(XMFLOAT4));
}

TestScene::~TestScene()
{
}

void TestScene::Update(float elapsed, float spf)
{
	dLight->Apply();

	cbEye->Write(&CameraMgr::Instance()->Main()->transform->GetPos());
	DX_DContext->PSSetConstantBuffers(SHADER_REG_CB_EYE, 1, cbEye->GetAddress());

	btn->Update(canvas);

	newObj->transform->Rotate(UP, spf);
	newObj->Update();
}
