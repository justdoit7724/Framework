#include "stdafx.h"
#include "UI.h"
#include "Shader.h"
#include "Quad.h"
#include "Camera.h"
#include "Game_info.h"
#include "ShaderFormat.h"
#include "ShaderReg.h"

#include "Transform.h"
#include "DepthStencilState.h"
#include "RasterizerState.h"
#include "BlendState.h"
#include "Mouse.h"

UI::UI(UICanvas* canvas, XMFLOAT2 pivot, XMFLOAT2 size, float zDepth, ID3D11ShaderResourceView * srv)
	:size(size), srv(srv)
{
	assert(0 <= zDepth && zDepth <= 1);

	canvas->Add(this);
	enabled = true;

	transp = 1;

	quad = new Quad();
	transform = new Transform();
	transform->SetScale(size.x, size.y, 1);
	transform->SetRot(-FORWARD, UP);
	transform->SetTranslation(pivot.x + size.x * 0.5f, (canvas->totalHeight -size.y * 0.5f) - pivot.y, zDepth);

	vs = new VShader("UIVS.cso", 
		Adv_ILayouts,
		ARRAYSIZE(Adv_ILayouts));
	ps = new PShader("UIPS.cso");
	vs->AddCB(0, 1, sizeof(SHADER_STD_TRANSF));
	ps->AddCB(0, 1, sizeof(float));

	ps->AddSRV(SHADER_REG_SRV_DIFFUSE, 1);

	D3D11_BLEND_DESC blend_desc;
	blend_desc.AlphaToCoverageEnable = false;
	blend_desc.IndependentBlendEnable = false;
	blend_desc.RenderTarget[0].BlendEnable = true;
	blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	blendState = new BlendState(&blend_desc);
	D3D11_DEPTH_STENCIL_DESC ds_desc;
	ZeroMemory(&ds_desc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	ds_desc.DepthEnable = true;
	ds_desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	ds_desc.StencilEnable = false;
	dsState = new DepthStencilState(&ds_desc);
	rsState = new RasterizerState(nullptr);
}

UI::~UI()
{
	delete quad;
	delete vs;
	delete ps;
	delete dsState;
	delete blendState;
}

void UI::Fade(float offset)
{
	transp += offset;
	
	transp = Clamp(0, 1, transp);
}

void UI::Update(const Camera* camera)
{

}

void UI::Render(const Camera* camera)const
{
	XMMATRIX vp = camera->VMat() * camera->ProjMat();

	vs->WriteCB(0, &SHADER_STD_TRANSF(transform->WorldMatrix(), vp));
	
	vs->Apply();
	DX_DContext->HSSetShader(nullptr, nullptr, 0);
	DX_DContext->DSSetShader(nullptr, nullptr, 0);
	DX_DContext->GSSetShader(nullptr, nullptr, 0);
	ps->WriteCB(0, &transp);
	ps->WriteSRV(SHADER_REG_SRV_DIFFUSE, srv);
	ps->Apply();
	blendState->Apply();
	dsState->Apply();
	rsState->Apply();
	quad->Apply();
}


UIButton::UIButton(UICanvas* canvas, UINT trigID, const void* trigData, XMFLOAT2 pivot, XMFLOAT2 size, ID3D11ShaderResourceView* idleSRV, ID3D11ShaderResourceView* hoverSRV, ID3D11ShaderResourceView* pressSRV)
	:UI(canvas, pivot, size, 0, nullptr), idleSRV(idleSRV), hoverSRV(hoverSRV), pressSRV(pressSRV)
{
	triggerID = trigID;
	triggerData = trigData;

	bound = Math::Plane(transform->GetPos(),
		transform->GetForward(),
		transform->GetUp(),
		size * 0.5f);

}

void UIButton::Update(const Camera* camera)
{
	srv = idleSRV;

	bound = Math::Plane(transform->GetPos(),
		transform->GetForward(),
		transform->GetUp(),
		size * 0.5f);

	Math::Ray ray;
	camera->Pick(&ray);

	XMFLOAT3 itsPt;
	if (Math::IntersectRayPlane(ray, bound, &itsPt))
	{
		switch (Mouse::Instance()->LeftState())
		{
		case MOUSE_STATE_DOWN:
		case MOUSE_STATE_PRESSING:
			srv = pressSRV;
			break;
		case MOUSE_STATE_RELEASE:
			srv = hoverSRV;
			break;
		case MOUSE_STATE_UP:
		{
			Notify(triggerID, triggerData);
		}
			break;
		}
	}

}
void UIButton::Render(const Camera* camera) const
{
	UI::Render(camera);
}
UICanvas::UICanvas(float width, float height)
	: totalWidth(width), totalHeight(height)
{
	camera = new Camera("UI", FRAME_KIND_ORTHOGONAL, SCREEN_WIDTH, SCREEN_HEIGHT, 0.1f, 10, NULL, NULL, true);
	camera->transform->SetTranslation(XMFLOAT3(width * 0.5f, height * 0.5f, 0));
	camera->transform->SetRot(FORWARD, UP);
	camera->Update();
}

UICanvas::~UICanvas()
{
	delete camera;
}

void UICanvas::Add(UI* ui)
{
	assert(UIs.count(ui) == 0);

	UIs.insert(ui);
}

void UICanvas::Update(float spf)
{
	for (auto ui : UIs)
	{
		if(ui->Enabled())
			ui->Update(camera);
	}
}

void UICanvas::Render(UINT sceneDepth)
{
	if (sceneDepth >= 1)
		return;


	for (auto ui : UIs)
	{
		if (ui->Enabled())
			ui->Render(camera);
	}
}
