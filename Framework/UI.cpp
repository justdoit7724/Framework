#include "UI.h"
#include "Shader.h"
#include "Quad.h"
#include "Camera.h"
#include "Game_info.h"
#include "ShaderFormat.h"

#include "Transform.h"
#include "DepthStencilState.h"
#include "BlendState.h"


UI::UI(float canvasWidth, float canvasHeight, XMFLOAT2 pivot, float width, float height, float zDepth, ID3D11ShaderResourceView * srv, UINT maxSliceIdx, UINT slicePerSec)
	:maxSliceIdx(maxSliceIdx), secPerSlice(1.0f / slicePerSec)
{
	assert(0 <= zDepth && zDepth <= 1);

	quad = new Quad();
	transform = new Transform();
	transform->SetScale(width, height, 1);
	transform->SetRot(-FORWARD, UP, -RIGHT);
	transform->SetTranslation(pivot.x + width * 0.5f, (canvasHeight - height * 0.5f) - pivot.y, zDepth);


	vs = new VShader("UIVS.cso", 
		Std_ILayouts,
		ARRAYSIZE(Std_ILayouts));
	hs = new HShader();
	ds = new DShader();
	gs = new GShader();
	ps = new PShader("UIPS.cso");
	vs->AddCB(0, 1, sizeof(SHADER_STD_TRANSF));

	ps->AddCB(0, 1, sizeof(float));
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	ps->AddSamp(0, 1, &samplerDesc);
	ps->AddSRV(0, 1);
	ps->WriteSRV(0, srv);

	D3D11_BLEND_DESC blend_desc;
	blend_desc.AlphaToCoverageEnable = false;
	blend_desc.IndependentBlendEnable = false;
	blend_desc.RenderTarget[0].BlendEnable = true;
	blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	blendState = new BlendState(&blend_desc);

	dsState = new DepthStencilState(nullptr);
}

UI::~UI()
{
	delete quad;
	delete vs;
	delete hs;
	delete ds;
	delete gs;
	delete ps;
	delete dsState;
	delete blendState;
	if(texSampState)
	texSampState->Release();
}

void UI::Update(float spf, const XMMATRIX& vpMat, const XMMATRIX& texMat)
{
	curTime += spf;
	if (curTime >= secPerSlice)
	{
		curSliceIdx = (curSliceIdx+1) % maxSliceIdx;
		curTime = 0;
	}

	vs->WriteCB(0,&SHADER_STD_TRANSF(transform->WorldMatrix(), vpMat, texMat));
	float fIdx = curSliceIdx;
	ps->WriteCB(0,&fIdx);
}

void UI::Render()
{
	vs->Apply();
	hs->Apply();
	ds->Apply();
	gs->Apply();
	ps->Apply();
	blendState->Apply();
	dsState->Apply();
	quad->Apply();
}

UICanvas::UICanvas(float width, float height)
	: totalWidth(width), totalHeight(height)
{
	camera = new Camera("UI", FRAME_KIND_ORTHOGONAL, width, height, 0.1f, 10, NULL, NULL, XMFLOAT3(width*0.5f, height*0.5f, -5), FORWARD, UP);
}

UICanvas::~UICanvas()
{
	while (!UIs.empty())
	{
		delete UIs.begin()->second;
		UIs.erase(UIs.begin());
	}
}

void UICanvas::Add(std::string id, XMFLOAT2 pivot, float width, float height, float zDepth, ID3D11ShaderResourceView* srv, UINT maxSliceIdx, UINT slicePerSec)
{
	if (UIs.find(id) == UIs.end())
	{
		UIs.insert(std::pair<std::string, UI*>(id, new UI(totalWidth, totalHeight, pivot,width,height, zDepth, srv, maxSliceIdx, slicePerSec)));
	}
}

void UICanvas::Remove(std::string id)
{
	if (UIs.find(id) != UIs.end())
	{
		delete UIs[id];
		UIs.erase(id);
	}
}

UI * UICanvas::Get(std::string id)
{
	auto i = UIs.find(id);

	assert(i != UIs.end());

	return i->second;
}

void UICanvas::Update(float spf)
{
	for (auto& ui : UIs)
	{
		ui.second->Update(spf, camera->VPMat(Z_ORDER_UI),XMMatrixIdentity());
	}
}

void UICanvas::Render()
{
	for (auto& ui : UIs)
	{
		ui.second->Render();
	}
}

