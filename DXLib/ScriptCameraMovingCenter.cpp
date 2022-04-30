#include "stdafx.h"
#include "ScriptCameraMovingCenter.h"
#include "Camera.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Timer.h"
#include "Transform.h"
#include "Object.h"

ScriptCameraMovingCenter::ScriptCameraMovingCenter(Script* owner)
	:SubScript(owner)
{
}

void ScriptCameraMovingCenter::Update()
{
	Transform* curTransform = (Transform*)m_script->m_object->GetComponent(CMPNT_KIND::Transform);

	XMFLOAT3 newPos = curTransform->GetPos();
	XMFLOAT3 right = curTransform->GetRight();
	XMFLOAT3 forward = curTransform->GetForward();
	const float speed = 7.5;
	const float spf = SGL_TIMER.SPF();

	if (SGL_Keyboard.GetState('A') == KEYBOARD_STATE::PRESSING) {

		newPos += -right * speed * spf;
	}
	else if (SGL_Keyboard.GetState('D') == KEYBOARD_STATE::PRESSING) {

		newPos += right * speed * spf;
	}
	if (SGL_Keyboard.GetState('S') == KEYBOARD_STATE::PRESSING) {

		newPos += -forward * speed * spf;
	}
	else if (SGL_Keyboard.GetState('W') == KEYBOARD_STATE::PRESSING) {

		newPos += forward * speed * spf;
	}
	static float angleX = 0;
	static float angleY = 0;
	static XMFLOAT2 prevMousePt;
	const float angleSpeed = 3.141592f * 0.2f;
	XMFLOAT2 scnPos = SGL_Mouse.GetScnPos();
	if (SGL_Mouse.GetClickState(FALSE) == MOUSE_STATE::PRESSING)
	{
		angleY += angleSpeed * spf * (scnPos.x - prevMousePt.x);
		angleX += angleSpeed * spf * (scnPos.y - prevMousePt.y);
	}
	prevMousePt = scnPos;
	const XMMATRIX rotMat = XMMatrixRotationX(angleX) * XMMatrixRotationY(angleY);
	curTransform->SetTranslation(newPos);
	XMFLOAT3 f = MultiplyDir(DIR_FORWARD, rotMat);
	XMFLOAT3 u = MultiplyDir(DIR_UP, rotMat);
	curTransform->SetRot(f, u);
}
