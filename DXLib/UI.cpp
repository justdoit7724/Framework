
#include "pch.h"

#include "UI.h"
#include "Shader.h"
#include "QuadMesh.h"
#include "Camera.h"

#include "ShaderFormat.h"
#include "ShaderReg.h"
#include "QuadCollider.h"
#include "Transform.h"
#include "DepthStencilState.h"
#include "RasterizerState.h"
#include "BlendState.h"

using namespace DX;

UI::UI(ID3D11Device* device, ID3D11DeviceContext* dContext, int iScnHeight, XMFLOAT2 pivot, XMFLOAT2 size, float zDepth, ID3D11ShaderResourceView * srv)
	:Object(device, dContext, "UI", nullptr, nullptr, "UIVS.cso", Std_ILayouts, ARRAYSIZE(Std_ILayouts),
		"","","","UIPS.cso"),
	size(size), srv(srv),
	mulColor(XMFLOAT4(1,1,1,1))
{
	mesh = std::make_shared<QuadMesh>(device);
	XMFLOAT3 pos = XMFLOAT3(pivot.x + size.x * 0.5f, (iScnHeight - size.y * 0.5f) - pivot.y, FLT_MIN + zDepth);
	collider = std::make_shared<QuadCollider>(pos);
	
	assert(0 <= zDepth && zDepth <= 1);

	transform->SetScale(size.x, size.y, 1);
	transform->SetRot(-FORWARD, UP);
	transform->SetTranslation(pos);

	vs->AddCB(device,0, 1, sizeof(SHADER_STD_TRANSF));
	ps->AddSRV(SHADER_REG_SRV_DIFFUSE, 1);
	ps->AddCB(device,SHADER_REG_CB_COLOR, 1, sizeof(XMFLOAT4));
	ps->WriteSRV(SHADER_REG_SRV_DIFFUSE, srv);
	ps->WriteCB(dContext, SHADER_REG_CB_COLOR, &mulColor);

	layer = LAYER_UI;
}

UI::~UI()
{
}

void UI::Update(ID3D11DeviceContext* dContext, int iScnWidth, int iScnHeight, XMFLOAT2 mousePos, UICanvas* canvas)
{

}

void UI::SetSRV(ID3D11ShaderResourceView* srv)
{
	this->srv = srv;
	ps->WriteSRV(SHADER_REG_SRV_DIFFUSE, srv);
}

void UI::SetMulColor(ID3D11DeviceContext* dContext, XMFLOAT3 c)
{
	mulColor.x = c.x;
	mulColor.y = c.y;
	mulColor.z = c.z;

	ps->WriteCB(dContext, SHADER_REG_CB_COLOR, &mulColor);
}

UIButton::UIButton(ID3D11Device* device, ID3D11DeviceContext* dContext, int iScnHeight, XMFLOAT2 pivot, XMFLOAT2 size, ID3D11ShaderResourceView* srv)
	:UI(device, dContext, iScnHeight, pivot, size, 0, srv)
{
	bound = Geometrics::Plane(transform->GetPos(),
		transform->GetForward(),
		transform->GetUp(),
		size * 0.5f);

}

void UIButton::Update(ID3D11DeviceContext* dContext, int iScnWidth, int iScnHeight, XMFLOAT2 mousePos, UICanvas* canvas)
{
	Object::Update();

	Geometrics::Ray ray;
	canvas->GetCamera()->Pick(iScnWidth , iScnHeight, mousePos ,&ray);


	SetMulColor(dContext,XMFLOAT3(1.0f, 1.0f, 1.0f));

	XMFLOAT3 hitPt;
	if (collider->IsHit(ray, &hitPt))
	{
		/*
		switch (Mouse::Instance()->LeftState())
		{
		case MOUSE_STATE_DOWN:
		case MOUSE_STATE_PRESSING:
			SetMulColor(XMFLOAT3(0.5f, 0.5f, 0.5f));
			break;
		case MOUSE_STATE_RELEASE:
			SetMulColor(XMFLOAT3(0.75f, 0.75f, 0.75f));
			break;
		case MOUSE_STATE_UP:
			Notify(notifyID, notifyData);
			break;
		}*/
	}
}
UICanvas::UICanvas(int iScnWidth, int iScnHeight)
	: totalWidth(iScnWidth), totalHeight(iScnHeight)
{
	camera = new Camera("UI", FRAME_KIND_ORTHOGONAL, totalWidth, totalHeight, 0.1f, 10, NULL, NULL, true);
	camera->transform->SetTranslation(XMFLOAT3(totalWidth * 0.5f, totalHeight * 0.5f, 0));
	camera->transform->SetRot(FORWARD, UP);
	camera->Update();
	camera->SetLayer(LAYER_UI);
}

UICanvas::~UICanvas()
{
	delete camera;
}

