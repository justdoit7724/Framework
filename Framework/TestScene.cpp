#include "stdafx.h"
#include "ShaderReg.h"
#include "TestScene.h"
#include "TextureMgr.h"
#include "stdafx.h"
#include "Object.h"
#include "Sphere.h"
#include "Transform.h"
#include "Buffer.h"
#include "Camera.h"
#include "Shader.h"
#include "CameraMgr.h"
#include "ShaderFormat.h"
#include "Light.h"

TestScene::TestScene()
{
	dLight = new DirectionalLight(
		XMFLOAT3(0.4, 0.4, 0.4),
		XMFLOAT3(0.7, 0.7, 0.7),
		XMFLOAT3(0.9, 0.9, 0.9),
		Normalize(XMFLOAT3(1, -0.5, 1)));

	std::vector<std::string> cmList;
	cmList.push_back("Data\\Texture\\cm_px.jpg");
	cmList.push_back("Data\\Texture\\cm_nx.jpg");
	cmList.push_back("Data\\Texture\\cm_py.jpg");
	cmList.push_back("Data\\Texture\\cm_ny.jpg");
	cmList.push_back("Data\\Texture\\cm_pz.jpg");
	cmList.push_back("Data\\Texture\\cm_nz.jpg");
	TextureMgr::Instance()->LoadCM("cm", cmList);
	ID3D11ShaderResourceView* cmSRV = TextureMgr::Instance()->Get("cm");
	DX_DContext->PSSetShaderResources(SHADER_REG_SRV_CM, 1, &cmSRV);
	TextureMgr::Instance()->Load("green", "Data\\Texture\\green_light.png");
	TextureMgr::Instance()->Load("normal", "Data\\Texture\\default_normal.png");
	TextureMgr::Instance()->Load("t1", "Data\\Texture\\t1.png");
	TextureMgr::Instance()->Load("t2", "Data\\Texture\\t2.png");
	TextureMgr::Instance()->Load("t3", "Data\\Texture\\t3.png");
	TextureMgr::Instance()->Load("t4", "Data\\Texture\\t4.png");
	TextureMgr::Instance()->Load("t5", "Data\\Texture\\t5.png");
	TextureMgr::Instance()->Load("t6", "Data\\Texture\\t6.png");

	std::shared_ptr<Sphere> sphere = std::make_shared<Sphere>(4);
	
	Object* newObj = new Object("Obj", sphere, sphere,
		TextureMgr::Instance()->Get("green"),
		TextureMgr::Instance()->Get("normal"));
	delete newObj->ps;
	newObj->ps = new PShader("PBRTestPS.cso");
	newObj->ps->AddCB(SHADER_REG_CB_MATERIAL, 1, sizeof(SHADER_MATERIAL));
	newObj->ps->AddSRV(SHADER_REG_SRV_DIFFUSE, 1);
	newObj->ps->AddSRV(SHADER_REG_SRV_NORMAL, 1);
	newObj->ps->AddSRV(SHADER_REG_SRV_METALLIC, 1);
	newObj->ps->AddSRV(SHADER_REG_SRV_ROUGHNESS, 1);
	newObj->ps->WriteSRV(SHADER_REG_SRV_DIFFUSE, TextureMgr::Instance()->Get("green"));
	newObj->ps->WriteSRV(SHADER_REG_SRV_NORMAL, TextureMgr::Instance()->Get("normal"));
	newObj->ps->WriteSRV(SHADER_REG_SRV_METALLIC, TextureMgr::Instance()->Get("t1"));
	newObj->ps->WriteSRV(SHADER_REG_SRV_ROUGHNESS, TextureMgr::Instance()->Get("t1"));
	newObj->ps->WriteCB(SHADER_REG_CB_MATERIAL, &SHADER_MATERIAL(XMFLOAT3(0.7, 0.7, 0.7), 0.5f, XMFLOAT3(0.5, 0.5, 0.5), XMFLOAT3(0.8, 0.8, 0.8)));

	newObj->transform->SetScale(XMFLOAT3(10,10,10));
	newObj->transform->SetTranslation(XMFLOAT3(0, 10, 0));
	AddObj(newObj);

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

	Scene::Update(elapsed, spf);
}
