#include "stdafx.h"
#include "UI.h"
#include "Shader.h"
#include "QuadMesh.h"
#include "Camera.h"
#include "Game_info.h"
#include "ShaderFormat.h"
#include "ShaderReg.h"
#include "QuadCollider.h"
#include "Transform.h"
#include "DepthStencilState.h"
#include "RasterizerState.h"
#include "BlendState.h"
#include "ObjectMgr.h"
#include "Mouse.h"

UI::UI(XMFLOAT2 pivot, XMFLOAT2 size, float zDepth, ID3D11ShaderResourceView * srv)
	:Object("UI", nullptr, nullptr, "UIVS.cso", Std_ILayouts, ARRAYSIZE(Std_ILayouts),
		"","","","UIPS.cso"),
	size(size), srv(srv)
{
	mesh = std::make_shared<QuadMesh>();
	XMFLOAT3 pos = XMFLOAT3(pivot.x + size.x * 0.5f, (SCREEN_HEIGHT - size.y * 0.5f) - pivot.y, FLT_MIN + zDepth);
	collider = std::make_shared<QuadCollider>(pos);
	
	assert(0 <= zDepth && zDepth <= 1);

	transform->SetScale(size.x, size.y, 1);
	transform->SetRot(-FORWARD, UP);
	transform->SetTranslation(pos);

	vs->AddCB(0, 1, sizeof(SHADER_STD_TRANSF));
	ps->AddSRV(SHADER_REG_SRV_DIFFUSE, 1);
	ps->WriteSRV(SHADER_REG_SRV_DIFFUSE, srv);

	layer = LAYER_UI;
}

UI::~UI()
{
}

void UI::Update(UICanvas* canvas)
{

}

void UI::SetSRV(ID3D11ShaderResourceView* srv)
{
	this->srv = srv;
	ps->WriteSRV(SHADER_REG_SRV_DIFFUSE, srv);
}


UIButton::UIButton(XMFLOAT2 pivot, XMFLOAT2 size, ID3D11ShaderResourceView* idleSRV, ID3D11ShaderResourceView* hoverSRV, ID3D11ShaderResourceView* pressSRV)
	:UI(pivot, size, 0, idleSRV), hoverSRV(hoverSRV), pressSRV(pressSRV)
{
	bound = Plane(transform->GetPos(),
		transform->GetForward(),
		transform->GetUp(),
		size * 0.5f);

}

void UIButton::Update(UICanvas* canvas)
{
	Object::Update();

	ID3D11ShaderResourceView* tempSRV = srv;

	Ray ray;
	canvas->GetCamera()->Pick(&ray);

	XMFLOAT3 hitPt;
	if (collider->IsHit(ray, &hitPt))
	{
		switch (Mouse::Instance()->LeftState())
		{
		case MOUSE_STATE_DOWN:
		case MOUSE_STATE_PRESSING:
			tempSRV = pressSRV;
			break;
		case MOUSE_STATE_RELEASE:
			tempSRV = hoverSRV;
			break;
		case MOUSE_STATE_UP:
			Notify(notifyID, notifyData);
			break;
		}
	}

	ps->WriteSRV(SHADER_REG_SRV_DIFFUSE, tempSRV);
}
UICanvas::UICanvas()
	: totalWidth(SCREEN_WIDTH), totalHeight(SCREEN_HEIGHT)
{
	camera = new Camera("UI", FRAME_KIND_ORTHOGONAL, SCREEN_WIDTH, SCREEN_HEIGHT, 0.1f, 10, NULL, NULL, true);
	camera->transform->SetTranslation(XMFLOAT3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0));
	camera->transform->SetRot(FORWARD, UP);
	camera->Update();
	camera->SetLayer(LAYER_UI);
}

UICanvas::~UICanvas()
{
	delete camera;
}

void UICanvas::Update()
{
}

