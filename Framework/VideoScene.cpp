#include "VideoScene.h"
#include "Object.h"
#include "Cube.h"
#include "Camera.h"
#include "TextureMgr.h"
#include "Transform.h"
#include "Timer.h"
#include "CameraMgr.h"
#include "Debugging.h"
#include "Shader.h"
#include "Light.h"

const float r = 1.0f;
const float angle = 1.57f;
const float n = 0.1f;
const float f = 70.0f;
VideoScene::VideoScene()
	:Scene("Video")
{
	Debugging::Instance()->DisableGrid();
	
	sceneCam = new Camera("videoScene", FRAME_KIND_PERSPECTIVE, NULL, NULL, n, f, angle, r, XMFLOAT3(0, 0, 0), FORWARD, UP);
	CameraMgr::Instance()->SetMain("DebugCamera");

	timer = new Timer();

	TextureMgr::Instance()->Load("r", "red_light.png", 1);
	TextureMgr::Instance()->Load("g", "green_light.png", 1);
	TextureMgr::Instance()->Load("b", "blue_light.png", 1);
	ID3D11ShaderResourceView* redSRV;
	ID3D11ShaderResourceView* greenSRV;
	ID3D11ShaderResourceView* blueSRV;
	TextureMgr::Instance()->Get("r", &redSRV);
	TextureMgr::Instance()->Get("g", &greenSRV);
	TextureMgr::Instance()->Get("b", &blueSRV);

	red = new Object(new Cube(), XMFLOAT3(1, 1, 1), XMFLOAT3(2, 2, 2), XMFLOAT3(0.3, 0.3, 0.3), 12, XMFLOAT3(0, 0, 0), redSRV, nullptr, nullptr, Z_ORDER_HIGHER);
	green = new Object(new Cube(), XMFLOAT3(1, 1, 1), XMFLOAT3(2, 2, 2), XMFLOAT3(0.3, 0.3, 0.3), 12, XMFLOAT3(0, 0, 0), greenSRV, nullptr, nullptr, Z_ORDER_STANDARD);
	green2 = new Object(new Cube(), XMFLOAT3(1, 1, 1), XMFLOAT3(2.5, 2.5, 2.5), XMFLOAT3(0.4, 0.4, 0.4), 12, XMFLOAT3(0, 0, 0), greenSRV, nullptr, nullptr, Z_ORDER_STANDARD);
	blue = new Object(new Cube(), XMFLOAT3(1, 1, 1), XMFLOAT3(2, 2, 2), XMFLOAT3(0.3, 0.3, 0.3), 12, XMFLOAT3(0, 0, 0), blueSRV, nullptr, nullptr, Z_ORDER_LOWER);

	red->transform->SetScale(5, 5, 5);
	red->transform->SetTranslation(0, 0, 30);
	green->transform->SetScale(5, 5, 5);
	green->transform->SetTranslation(0, 0, 15);
	green2->transform->SetScale(5, 5, 5);
	green2->transform->SetTranslation(0, 0, 60);
	blue->transform->SetScale(5, 5, 5);
	blue->transform->SetTranslation(0, 0, 45);

	AddObj(red);
	AddObj(green);
	AddObj(green2);
	AddObj(blue);

	redMat = XMMatrixIdentity();
	greenMat = XMMatrixIdentity();
	blueMat = XMMatrixIdentity();

	curTime = 0;
	totalTime = 7.5f;
}

void VideoScene::Logic_Update()
{
	Debugging::Instance()->PtLine(XMFLOAT3(0, 0, 0), XMFLOAT3(6, 0, 0), Colors::Red);
	Debugging::Instance()->PtLine(XMFLOAT3(0, 0, 0), XMFLOAT3(0, 6, 0), Colors::Green);
	Debugging::Instance()->PtLine(XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 6), Colors::Blue);

	float elapsed = timer->Elapsed();
	XMMATRIX rot1 = XMMatrixRotationX(cos(elapsed * 0.3f)) * XMMatrixRotationY(sin(elapsed * 0.5f));
	XMMATRIX rot2 = XMMatrixRotationY(sin(elapsed * 0.2f)) * XMMatrixRotationZ(cos(elapsed * 0.4f));
	XMMATRIX rot3 = XMMatrixRotationZ(cos(elapsed * 0.4f)) * XMMatrixRotationY(sin(elapsed * 0.3f));
	XMMATRIX rot4 = XMMatrixRotationX(sin(elapsed * 0.2f)) * XMMatrixRotationZ(cos(elapsed * 0.5f));
	red->transform->SetRot(FORWARD * rot1, UP * rot1, RIGHT * rot1);
	green->transform->SetRot(FORWARD * rot2, UP * rot2, RIGHT * rot2);
	green2->transform->SetRot(FORWARD * rot3, UP * rot3, RIGHT * rot3);
	blue->transform->SetRot(FORWARD * rot4, UP * rot4, RIGHT * rot4);
}

XMFLOAT3 Lerp(XMFLOAT3 v1, XMFLOAT3 v2, float t)
{
	return v1 + ((v2 - v1) * t);
}
void LineDrawing(Camera* camera, float t)
{
	XMFLOAT3 p = camera->transform->GetPos();
	XMFLOAT3 forward = camera->transform->GetForward();
	XMFLOAT3 up = camera->transform->GetUp();
	XMFLOAT3 right = camera->transform->GetRight();
	float tri = tan(angle * 0.5f);
	float nY = tri * n;
	float nX = nY * r;
	float fY = tri * f;
	float fX = fY * r;
	XMFLOAT3 sTL = Lerp(p + right * -nX + up * nY + forward * n, XMFLOAT3(-1,1,0),t);
	XMFLOAT3 sTR = Lerp(p + right * nX + up * nY + forward * n, XMFLOAT3(1, 1, 0),t);
	XMFLOAT3 sBL = Lerp(p + right * -nX + up * -nY + forward * n, XMFLOAT3(-1, -1, 0),t);
	XMFLOAT3 sBR = Lerp(p + right * nX + up * -nY + forward * n, XMFLOAT3(1, -1, 0),t);
	XMFLOAT3 eTL = Lerp(p + right * -fX + up * fY + forward * f, XMFLOAT3(-1, 1, 1),t);
	XMFLOAT3 eTR = Lerp(p + right * fX + up * fY + forward * f, XMFLOAT3(1, 1, 1),t);
	XMFLOAT3 eBL = Lerp(p + right * -fX + up * -fY + forward * f, XMFLOAT3(-1, -1, 1),t);
	XMFLOAT3 eBR = Lerp(p + right * fX + up * -fY + forward * f, XMFLOAT3(1, -1, 1),t);

	Debugging::Instance()->PtLine(sTL, sTR);
	Debugging::Instance()->PtLine(sTR, sBR);
	Debugging::Instance()->PtLine(sBR, sBL);
	Debugging::Instance()->PtLine(sBL, sTL);

	Debugging::Instance()->PtLine(sTL, eTL);
	Debugging::Instance()->PtLine(sTR, eTR);
	Debugging::Instance()->PtLine(sBL, eBL);
	Debugging::Instance()->PtLine(sBR, eBR);

	Debugging::Instance()->PtLine(eTL, eTR);
	Debugging::Instance()->PtLine(eTR, eBR);
	Debugging::Instance()->PtLine(eBR, eBL);
	Debugging::Instance()->PtLine(eBL, eTL);
}

void VideoScene::Render_Update(const Camera* camera, float elapsed, float spf)
{
	curTime = min(curTime+spf,totalTime);
	float t = curTime / totalTime;
	LineDrawing(sceneCam, t);
	float scaleX = Lerp(t,1, 1.0f / (r * tan(angle * 0.5f)));
	float scaleY = Lerp(t,1, 1.0f / tan(angle * 0.5f));
	float hd = Lerp(t,1, 0);
	float hz = Lerp(t,0, 1);
	float minD = 0.2f;
	float maxD = 0.4f;
	redMat = XMMATRIX(
		scaleX, 0, 0, 0,
		0, scaleY, 0, 0,
		0, 0, Lerp(t,1,maxD * f / (f - n) - minD * n / (f - n)), hz,
		0, 0, Lerp(t,0,n * (minD - maxD * f / (f - n) - minD * n / (f - n))), hd
	);
	minD = 0.4f;
	maxD = 0.6f;
	greenMat = XMMATRIX(
		scaleX, 0, 0, 0,
		0, scaleY, 0, 0,
		0, 0, Lerp(t,1,maxD * f / (f - n) - minD * n / (f - n)), hz,
		0, 0, Lerp(t,0,n * (minD - maxD * f / (f - n) - minD * n / (f - n))), hd
	);
	minD = 0.6f;
	maxD = 0.8f;
	blueMat = XMMATRIX(
		scaleX, 0, 0, 0,
		0, scaleY, 0, 0,
		0, 0, Lerp(t,1,maxD * f / (f - n) - minD * n / (f - n)), hz,
		0, 0, Lerp(t,0,n * (minD - maxD * f / (f - n) - minD * n / (f - n))), hd
	);

	const SHADER_STD_TRANSF t1(red->transform->WorldMatrix()*redMat, camera->VPMat(Z_ORDER_STANDARD), XMMatrixIdentity());

	red->vs->WriteCB(0, (void*)(&t1));
	red->ps->WriteCB(0, (void*)DirectionalLight::Data());
	red->ps->WriteCB(1, (void*)PointLight::Data());
	red->ps->WriteCB(2, (void*)SpotLight::Data());
	red->ps->WriteCB(3, &XMFLOAT4(camera->transform->GetPos().x, camera->transform->GetPos().y, camera->transform->GetPos().z, 0));
	red->ps->WriteCB(5, &elapsed);


	const SHADER_STD_TRANSF t2(green->transform->WorldMatrix()*greenMat, camera->VPMat(Z_ORDER_STANDARD), XMMatrixIdentity());

	green->vs->WriteCB(0, (void*)(&t2));
	green->ps->WriteCB(0, (void*)DirectionalLight::Data());
	green->ps->WriteCB(1, (void*)PointLight::Data());
	green->ps->WriteCB(2, (void*)SpotLight::Data());
	green->ps->WriteCB(3, &XMFLOAT4(camera->transform->GetPos().x, camera->transform->GetPos().y, camera->transform->GetPos().z, 0));
	green->ps->WriteCB(5, &elapsed);


	const SHADER_STD_TRANSF t3(green2->transform->WorldMatrix()* greenMat, camera->VPMat(Z_ORDER_STANDARD), XMMatrixIdentity());

	green2->vs->WriteCB(0, (void*)(&t3));
	green2->ps->WriteCB(0, (void*)DirectionalLight::Data());
	green2->ps->WriteCB(1, (void*)PointLight::Data());
	green2->ps->WriteCB(2, (void*)SpotLight::Data());
	green2->ps->WriteCB(3, &XMFLOAT4(camera->transform->GetPos().x, camera->transform->GetPos().y, camera->transform->GetPos().z, 0));
	green2->ps->WriteCB(5, &elapsed);


	const SHADER_STD_TRANSF t4(blue->transform->WorldMatrix()* blueMat, camera->VPMat(Z_ORDER_STANDARD), XMMatrixIdentity());

	blue->vs->WriteCB(0, (void*)(&t4));
	blue->ps->WriteCB(0, (void*)DirectionalLight::Data());
	blue->ps->WriteCB(1, (void*)PointLight::Data());
	blue->ps->WriteCB(2, (void*)SpotLight::Data());
	blue->ps->WriteCB(3, &XMFLOAT4(camera->transform->GetPos().x, camera->transform->GetPos().y, camera->transform->GetPos().z, 0));
	blue->ps->WriteCB(5, &elapsed);
}

void VideoScene::Render() const
{
	Scene::Render();
}
