#include "PlayScene.h"
#include "Object.h"
#include "SphereMesh.h"
#include "TextureMgr.h"
#include "Transform.h"
#include "Light.h"
#include "CameraMgr.h"
#include "Buffer.h"
#include "Camera.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Debugging.h"
#include "LineMesh.h"
#include "ShaderFormat.h"
#include "Geometrics.h"
#include "UnlitObj.h"
#include "MeshLoader.h"
#include "ShaderReg.h"

PlayScene::PlayScene()
	:camDist(300)
{
	dLight = new DirectionalLight(
		XMFLOAT3(0.5, 0.5, 0.5),
		XMFLOAT3(0.8, 0.8, 0.8),
		XMFLOAT3(0.4, 0.4, 0.4),
		Normalize(XMFLOAT3(-1,-0.5f,-1))
	);

	checkCam = new Camera("CheckCam", FRAME_KIND_PERSPECTIVE, NULL, NULL, 1.0f, 1000.0f, XM_PIDIV2, 1, true);
	checkCam->transform->SetTranslation(0, 0, -camDist);
	checkCam->SubtractLayer(LAYER_UI);
	CameraMgr::Instance()->SetMain(checkCam);
	Debugging::Instance()->SetCamera(checkCam);

	cbEye = new Buffer(sizeof(XMFLOAT4));

	TextureMgr::Instance()->Load("white", "DXFramework\\Data\\Texture\\white.png");
	TextureMgr::Instance()->Load("green", "DXFramework\\Data\\Texture\\green_light.png");
	TextureMgr::Instance()->Load("sample", "DXFramework\\Data\\Texture\\sample.jpg");

	CreateModel("Hand.ply");
	CreateGrid();
}

void PlayScene::Update(float elapsed, float spf)
{
	XMFLOAT3 eye = CameraMgr::Instance()->Main()->transform->GetPos();
	cbEye->Write(&eye);
	DX_DContext->PSSetConstantBuffers(SHADER_REG_CB_EYE, 1, cbEye->GetAddress());
	dLight->Apply();

	mainObj->transform->SetRot(XMFLOAT3(cos(elapsed/2.0f), 0, sin(elapsed/2.0f)), UP);

	CamMove(spf);
}

void PlayScene::CreateModel(std::string filename)
{
	if (mainObj)
		delete mainObj;

	std::vector<Mesh*> meshes;
	MeshLoader::Load(&meshes, "DXFramework\\Data\\Model\\PLY\\", filename);

	mainObj = new Object("Test Obj", std::unique_ptr<Mesh>(meshes[0]), nullptr, TextureMgr::Instance()->Get("white"));
	mainObj->Update();
	
}

void PlayScene::CreateGrid()
{
	if (grid3D)
	{
		delete grid3D;
	}

	float lowY = mainObj? mainObj->Bound().p.y - mainObj->Bound().rad : 0;
	const float length = 300;
	const int gridCount = 50;
	const float interval = length / gridCount;
	const XMFLOAT3 startP = XMFLOAT3(-gridCount / 2.0f, 0, -gridCount / 2.0f)*interval;
	auto lineMesh = std::make_shared<LineMesh>();
	for (int x = 0; x < gridCount; ++x)
	{
		XMFLOAT3 p1 = startP + XMFLOAT3(x, 0, 0)*interval;
		XMFLOAT3 p2 = startP + XMFLOAT3(x, 0, gridCount)*interval;
		p1.y += lowY;
		p2.y += lowY;
		lineMesh->Add(p1, p2);
	}
	for (int z = 0; z < gridCount; ++z)
	{
		XMFLOAT3 p1 = startP + XMFLOAT3(0, 0, z)*interval;
		XMFLOAT3 p2 = startP + XMFLOAT3(gridCount, 0, z)*interval;
		p1.y += lowY;
		p2.y += lowY;
		lineMesh->Add(p1, p2);
	}
	lineMesh->Generate();
	grid3D = new UnlitObj("3D grid", lineMesh, nullptr, Colors::White);
}

void PlayScene::CamMove(float spf)
{
	XMFLOAT3 right = checkCam->transform->GetRight();
	XMFLOAT3 forward = checkCam->transform->GetForward();
	const float speed = 30;

	static float angleX = 0;
	static float angleY = 0;
	static XMFLOAT2 prevMousePt = XMFLOAT2(0, 0);
	const float angleSpeed = 3.141592f*0.25f;
	XMFLOAT2 mPt = Mouse::Instance()->Pos();
	switch (Mouse::Instance()->RightState())
	{
	case MOUSE_STATE_PRESSING:
		angleY = angleY + angleSpeed * spf * (mPt.x - prevMousePt.x);
		angleX = Clamp(-XM_PIDIV2 * 0.9f, XM_PIDIV2*0.9f, angleX + angleSpeed * spf * (mPt.y - prevMousePt.y));
		break;
	}
	camDist = Clamp(20, 700, camDist - Mouse::Instance()->GetWheel() * 20);
	prevMousePt.x = mPt.x;
	prevMousePt.y = mPt.y;

	XMFLOAT3 newPos = XMFLOAT3(0, sin(angleX), -cos(angleX))* camDist;

	XMMATRIX rotY(
		cos(angleY), 0, -sin(angleY), 0,
		0, 1, 0, 0,
		sin(angleY), 0, cos(angleY), 0,
		0, 0, 0, 1
	);
	newPos = Multiply(newPos, rotY);

	XMFLOAT3 newLook = -Normalize(newPos);
	XMFLOAT3 newRight = Normalize(Cross(UP, newLook));
	XMFLOAT3 newUp = Cross(newLook, newRight);

	checkCam->transform->SetTranslation(newPos);

	checkCam->transform->SetRot(newLook, newUp, newRight);
	checkCam->Update();
}
