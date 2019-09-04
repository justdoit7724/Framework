#include "UI.h"
#include "Shader.h"
#include "Quad.h"
#include "Camera.h"
#include "Game_info.h"
#include "CustomFormat.h"
#include "Network.h"
#include "Transform.h"

UI::UI(ID3D11Device* device, float canvasWidth, float canvasHeight, XMFLOAT2 pivot, float width, float height, float zDepth, ID3D11ShaderResourceView * srv, UINT maxSliceIdx, UINT slicePerSec)
	:srv(srv), maxSliceIdx(maxSliceIdx), secPerSlice(1.0f/ slicePerSec)
{
	assert(0 <= zDepth && zDepth <= 1);

	quad = new Quad(device);
	transform = new Transform();
	transform->SetScale(width, height, 1);
	transform->SetRot(-FORWARD, UP);
	transform->SetTranslation(pivot.x + width * 0.5f, (canvasHeight - height * 0.5f)- pivot.y, zDepth);

	shader = new VPShader(device, L"UIVS.cso", L"UIPS.cso", std_ILayouts, ARRAYSIZE(std_ILayouts));

	cb_vs_property = new ConstantBuffer<VS_Property>(device);
	cb_ps_sliceIdx = new ConstantBuffer<float>(device);

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	r_assert(
		device->CreateSamplerState(&samplerDesc, &texSampState)
	);
}

UI::~UI()
{
	delete quad;
	delete shader;
	delete cb_vs_property;
	delete cb_ps_sliceIdx;
	texSampState->Release();
}

void UI::Update(float spf)
{
	curTime += spf;
	if (curTime >= secPerSlice)
	{
		if (++curSliceIdx >= maxSliceIdx)
			curSliceIdx = 0;
		curTime = 0;
	}
}

void UI::Render(ID3D11DeviceContext* dContext, const XMMATRIX& vpMat)
{
	shader->SetPipline(dContext);
	cb_vs_property->VSSetData(dContext, &VS_Property(transform, vpMat));
	float tempSliceIdx = curSliceIdx;
	cb_ps_sliceIdx->PSSetData(dContext, &tempSliceIdx);
	dContext->PSSetShaderResources(0, 1, &srv);
	dContext->PSSetSamplers(0, 1, &texSampState);
	quad->Render(dContext);
}

UICanvas::UICanvas(ID3D11Device* device, float width, float height)
	: totalWidth(width), totalHeight(height)
{
	Debugging::Line(29035, XMFLOAT3(0, height, 0), XMFLOAT3(width, height, 0), Colors::Cyan);
	Debugging::Line(29036, XMFLOAT3(width, height, 0), XMFLOAT3(width, 0, 0), Colors::Cyan);

	camera = new Camera(FRAME_KIND_ORTHOGONAL, width, height, 0.1f, 10, NULL, NULL, XMFLOAT3(width*0.5f, height*0.5f, -5), FORWARD, UP);
}

UICanvas::~UICanvas()
{
	while (!UIs.empty())
	{
		delete UIs.begin()->second;
		UIs.erase(UIs.begin());
	}
}

void UICanvas::Add(ID3D11Device* device, std::string id, XMFLOAT2 pivot, float width, float height, float zDepth, ID3D11ShaderResourceView*const srv, UINT maxSliceIdx, UINT slicePerSec)
{
	if (UIs.find(id) == UIs.end())
	{
		UIs.insert(std::pair<std::string, UI*>(id, new UI(device, totalWidth, totalHeight, pivot,width,height, zDepth, srv, maxSliceIdx, slicePerSec)));
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

void UICanvas::Update(float spf)
{
	for (auto& ui : UIs)
	{
		ui.second->Update(spf);
	}
}

void UICanvas::Render(IGraphic* graphic)
{
	XMMATRIX vpMat = camera->VPMat(Z_ORDER_UI);

	for (auto& ui : UIs)
	{
		graphic->SetDepthStencilState();
		graphic->SetRasterizerState();

		ui.second->Render(graphic->DContext(), vpMat);
	}
}

