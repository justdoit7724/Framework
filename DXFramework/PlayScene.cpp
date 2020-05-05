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
#include "UI.h"
#include "MeshLoader.h"
#include "ShaderReg.h"

PlayScene::PlayScene()
	:camDist(300), camViewRad(XM_PIDIV2)
{
	dLight = new DirectionalLight(
		XMFLOAT3(0.5, 0.5, 0.5),
		XMFLOAT3(0.8, 0.8, 0.8),
		XMFLOAT3(0.4, 0.4, 0.4),
		Normalize(XMFLOAT3(-1,-0.5f,-1))
	);

	checkCam = new Camera("CheckCam", FRAME_KIND_PERSPECTIVE, NULL, NULL, 1.0f, 1000.0f, camViewRad, 1, true);
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

	mainObj->transform->SetRot(XMFLOAT3(cos(elapsed/10.0f), 0, sin(elapsed/10.0f)), UP);

	if (Keyboard::GetKey('A')==KeyState_Up)
	{
		Lerp2CamX();
	}
	else if (Keyboard::GetKey('W') == KeyState_Up)
	{
		Lerp2CamY();
	}
	else if (Keyboard::GetKey('D') == KeyState_Up)
	{
		Lerp2CamZ();
	}

	CamMove(spf);
}

void PlayScene::CreateModel(std::string filename)
{
	if (mainObj)
		delete mainObj;

	/*std::vector<Mesh*> meshes;
	MeshLoader::Load(&meshes, "DXFramework\\Data\\Model\\PLY\\", filename);*/

	//mainObj = new Object("Test Obj", std::shared_ptr<Mesh>(meshes[0]), nullptr, TextureMgr::Instance()->Get("white"));

	mainObj = new Object("Test Obj", std::make_shared<SphereMesh>(0), nullptr, TextureMgr::Instance()->Get("white"));
	mainObj->transform->SetScale(50, 50, 50);
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

void PlayScene::Lerp2CamX()
{
	if (isLerping)
		return;
	isLerping = true;
	curLerpTime = 0;
	startAngleX = camAngleX;
	startAngleY = camAngleY;
	lerpAngleX = 0;
	lerpAngleY = -XM_PIDIV4 * 2;
	XMFLOAT2 topdownPos = XMFLOAT2(checkCam->transform->GetPos().x, checkCam->transform->GetPos().z);
	if (Dot(topdownPos, XMFLOAT2(0,1)) > 0 && startAngleY >= 0)
	{
		lerpAngleY = XM_PIDIV4 * 6;
	}
}

void PlayScene::Lerp2CamY()
{
	if (isLerping)
		return;
	isLerping = true;
	curLerpTime = 0;
	startAngleX = camAngleX;
	startAngleY = camAngleY;
	lerpAngleX = XM_PIDIV2*0.99f;
	lerpAngleY = camAngleY;

}

void PlayScene::Lerp2CamZ()
{
	if (isLerping)
		return;
	isLerping = true;
	curLerpTime = 0;
	startAngleX = camAngleX;
	startAngleY = camAngleY;
	lerpAngleX = 0;
	lerpAngleY = -XM_PIDIV4 * 4;
	XMFLOAT2 topdownPos = XMFLOAT2(checkCam->transform->GetPos().x, checkCam->transform->GetPos().z);
	if (Dot(topdownPos, XMFLOAT2(-1, 0)) > 0 && startAngleY >= 0)
	{
		lerpAngleY = XM_PIDIV4 * 4;
	}
	else
	{
		if (startAngleY >= 0)
			lerpAngleY = XM_PIDIV4 * 4;
		else
			lerpAngleY = -XM_PIDIV4 * 4;
	}
}

void PlayScene::Lerp2Perspective()
{
	if (isLerping)
		return;
	isLerping = true;
	curLerpTime = 0;
}

void PlayScene::Lerp2Orthogonal()
{
	if (isLerping)
		return;
	isLerping = true;
	curLerpTime = 0;
}

void PlayScene::CamMove(float spf)
{


	if (!isLerping)
	{
		XMFLOAT3 right = checkCam->transform->GetRight();
		XMFLOAT3 forward = checkCam->transform->GetForward();
		const float speed = 30;

		static XMFLOAT2 prevMousePt = XMFLOAT2(0, 0);
		const float angleSpeed = 3.141592f * 0.25f;
		XMFLOAT2 mPt = Mouse::Instance()->Pos();
		switch (Mouse::Instance()->RightState())
		{
		case MOUSE_STATE_PRESSING:
			camAngleY = camAngleY + angleSpeed * spf * (mPt.x - prevMousePt.x);
			camAngleX = Clamp(-XM_PIDIV2*0.99f, XM_PIDIV2*0.99f, camAngleX + angleSpeed * spf * (mPt.y - prevMousePt.y));
			break;
		}
		if (camAngleY > XM_PI*2) camAngleY -= XM_PI*2;
		if (camAngleY < -XM_PI*2) camAngleY += XM_PI*2;

		camViewRad = Clamp(XM_PI * 0.1f, XM_PI * 0.6f, checkCam->GetVRad() - Mouse::Instance()->GetWheel() * 0.1f);
		prevMousePt.x = mPt.x;
		prevMousePt.y = mPt.y;
	}
	else
	{
		curLerpTime += spf;
		float t = min(curLerpTime / lerpDuration, 1);

		camAngleX = Lerp(startAngleX, lerpAngleX, t);
		camAngleY = Lerp(startAngleY, lerpAngleY, t);

		if (t >= 1)
		{
			isLerping = false;
		}

		
	}

	XMMATRIX rotY(
		cos(camAngleY), 0, -sin(camAngleY), 0,
		0, 1, 0, 0,
		sin(camAngleY), 0, cos(camAngleY), 0,
		0, 0, 0, 1
	);
	XMFLOAT3 newPos = Multiply(XMFLOAT3(0, sin(camAngleX), -cos(camAngleX)) * camDist, rotY);

	XMFLOAT3 newForward = -Normalize(newPos);
	XMFLOAT3 newRight = Normalize(Cross(UP, newForward));
	XMFLOAT3 newUp = Cross(newForward, newRight);

	checkCam->SetFrame(FRAME_KIND_PERSPECTIVE, XMFLOAT2(WND_WIDTH, WND_HEIGHT), checkCam->GetN(), checkCam->GetF(), camViewRad, checkCam->GetAspectRatio());
	checkCam->transform->SetTranslation(newPos);
	checkCam->transform->SetRot(newForward, newUp, newRight);
	checkCam->Update();
}
