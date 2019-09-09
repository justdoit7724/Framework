#include "Debugging.h"
#include <string>
#include "Shader.h"
#include "Camera.h"
#include "Graphic.h"
#include "CustomFormat.h"

#include "Cube.h"
#include "Sphere.h"

std::vector<Debugging::ScreenTextInfo> Debugging::texts;
std::unordered_map<UINT, Debugging::MarkInfo> Debugging::marks;
std::unordered_map<UINT, Debugging::LineInfo> Debugging::lines;
ComPtr<ID3D11Buffer> Debugging::lineVB = nullptr;
ComPtr<ID3D11Buffer> Debugging::gridVB = nullptr;
ComPtr<ID3D11Buffer> Debugging::originVB = nullptr;
UINT Debugging::gridVerticeCount=0;
std::unique_ptr<VPShader> Debugging::shader = nullptr;
std::unique_ptr<ConstantBuffer<VS_Property>> Debugging::cb_transformation=nullptr;
std::unique_ptr<ConstantBuffer<XMVECTOR>> Debugging::cb_color=nullptr;
std::unique_ptr<DirectX::SpriteBatch> Debugging::spriteBatch=nullptr;
std::unique_ptr<DirectX::SpriteFont> Debugging::spriteFont=nullptr;
ComPtr<ID3D11BlendState> Debugging::blendState=nullptr;

float Debugging::gridInterval;

const XMMATRIX textMat = XMMATRIX(
	SCREEN_WIDTH/2.0f, 0, 0, 0,
	0, -SCREEN_HEIGHT/2.0f, 0, 0,
	0, 0, 1, 0,
	SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0, 1);
void Debugging::Draw(const std::string tex, const float x, const float y, const XMVECTORF32 _color, const float _scale)
{
	ScreenTextInfo newText;
	newText.tex = tex;
	newText.pos = XMFLOAT3(x,y,1);
	newText.color = _color;
	newText.scale = _scale;
	newText.is3D = false;

	texts.push_back(newText);
}
void Debugging::Draw(const int tex, const float x, const float y, const XMVECTORF32 _color, const float _scale)
{
	Draw(std::to_string(tex), x,y, _color, _scale);
}
void Debugging::Draw(float tex, const float x, const float y, XMVECTORF32 _color, float _scale)
{
	Draw(std::to_string(tex), x,y, _color, _scale);
}
void Debugging::Draw(std::string title, XMFLOAT3 v, const float x, const float y, XMVECTORF32 _color, float _scale)
{
	Draw(title + "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + ")", x,y, _color, _scale);
}
void Debugging::Draw3D(const std::string tex, const XMFLOAT3 _pos, const XMVECTORF32 _color, const float _scale)
{
	ScreenTextInfo newText;
	newText.tex = tex;
	newText.pos = _pos;
	newText.color = _color;
	newText.scale = _scale;
	newText.is3D = true;

	texts.push_back(newText);
}
void Debugging::Draw3D(const int tex, XMFLOAT3 _pos, const XMVECTORF32 _color, const float _scale)
{
	Draw3D(std::to_string(tex), _pos, _color, _scale);
}
void Debugging::Draw3D(float tex, XMFLOAT3 _pos, XMVECTORF32 _color, float _scale)
{
	Draw3D(std::to_string(tex), _pos, _color, _scale);
}
void Debugging::Draw3D(std::string title, XMFLOAT3 v, XMFLOAT3 _pos, XMVECTORF32 _color, float _scale)
{
	Draw3D(title + "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + ")", _pos, _color, _scale);
}
void Debugging::Mark(const UINT key, XMFLOAT3 pos, float radius, XMVECTORF32 color)
{
	if (marks.find(key) == marks.end())
	{
		/*Sphere* newGeom = new Sphere(device, 4);
		newGeom->SetTranslation(pos);
		newGeom->SetScale(radius, radius, radius);
		
		marks.insert(std::pair<UINT, MarkInfo>(key, MarkInfo(newGeom, color)));*/
	}
	else {
		/*marks[key].geom->SetTranslation(pos);
		marks[key].geom->SetScale(radius, radius, radius);*/
	}
}

void Debugging::Line(const UINT key, XMFLOAT3 p1, XMFLOAT3 p2, XMVECTORF32 color)
{
	if (lines.find(key) == lines.end())
	{
		lines.insert(std::pair<UINT, LineInfo>(key, LineInfo(p1,p2,color)));
	}
	else {
		lines[key].p1 = p1;
		lines[key].p2 = p2;
		lines[key].color = color;
	}
}

void Debugging::EnableGrid(float interval, int num)
{
	assert(num % 2 == 0);

	const float LENGTH = interval * num;
	const float HLENGTH = LENGTH * 0.5f;
	std::vector<Vertex> originVertice;
	std::vector<Vertex> gridVertice;
	// center indicators
	originVertice.push_back(XMFLOAT3(0, 0, 0));
	originVertice.push_back(XMFLOAT3(HLENGTH, 0, 0));
	originVertice.push_back(XMFLOAT3(0, 0, 0));
	originVertice.push_back(XMFLOAT3(0, 999, 0));
	originVertice.push_back(XMFLOAT3(0, 0, 0));
	originVertice.push_back(XMFLOAT3(0, 0, HLENGTH));
	for (int i = 0; i < num; ++i)
	{
		if (i == num * 0.5f)
			continue;

		// x
		gridVertice.push_back(XMFLOAT3(-HLENGTH, 0, -HLENGTH + interval * i));
		gridVertice.push_back(XMFLOAT3(HLENGTH, 0, -HLENGTH + interval * i));
		// z
		gridVertice.push_back(XMFLOAT3(-HLENGTH + interval * i, 0, -HLENGTH));
		gridVertice.push_back(XMFLOAT3(-HLENGTH + interval * i, 0, HLENGTH));
	}
	gridVertice.push_back(XMFLOAT3(0, 0, 0));
	gridVertice.push_back(XMFLOAT3(-HLENGTH, 0, 0));
	gridVertice.push_back(XMFLOAT3(0, 0, 0));
	gridVertice.push_back(XMFLOAT3(0, 0, -HLENGTH));
	gridVerticeCount = gridVertice.size();

	D3D11_BUFFER_DESC originVB_desc;
	originVB_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	originVB_desc.ByteWidth = sizeof(Vertex)*originVertice.size();
	originVB_desc.CPUAccessFlags = 0;
	originVB_desc.MiscFlags = 0;
	originVB_desc.StructureByteStride = 0;
	originVB_desc.Usage = D3D11_USAGE_IMMUTABLE;
	D3D11_SUBRESOURCE_DATA originVB_data;
	originVB_data.pSysMem = originVertice.data();
	r_assert(device->CreateBuffer(&originVB_desc, &originVB_data, originVB.ReleaseAndGetAddressOf()));
	D3D11_BUFFER_DESC gridVB_desc;
	gridVB_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	gridVB_desc.ByteWidth = sizeof(Vertex)*gridVertice.size();
	gridVB_desc.CPUAccessFlags = 0;
	gridVB_desc.MiscFlags = 0;
	gridVB_desc.StructureByteStride = 0;
	gridVB_desc.Usage = D3D11_USAGE_IMMUTABLE;
	D3D11_SUBRESOURCE_DATA gridVB_data;
	gridVB_data.pSysMem = gridVertice.data();
	r_assert(device->CreateBuffer(&gridVB_desc, &gridVB_data, gridVB.ReleaseAndGetAddressOf()));
}

void Debugging::DisableGrid()
{
	gridVB.ReleaseAndGetAddressOf();
	gridVB = nullptr;
}

void Debugging::Render(Camera* camera)
{
	XMMATRIX vpMat = camera->ViewMat() * camera->ProjMat(Z_ORDER_STANDARD);

	if (blendState == nullptr)
	{
		D3D11_BLEND_DESC blend_desc;
		blend_desc.AlphaToCoverageEnable = false;
		blend_desc.IndependentBlendEnable = false;
		blend_desc.RenderTarget[0].BlendEnable = true;
		blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
		blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		r_assert(
			device->CreateBlendState(&blend_desc, blendState.GetAddressOf())
		);
	}
	dContext->OMSetBlendState(blendState.Get(), nullptr, 1);

	#pragma region Marks

	if (shader == nullptr)
	{
		shader.reset(new VPShader(device,L"MarkVS.cso", L"MarkPS.cso", std_ILayouts, ARRAYSIZE(std_ILayouts)));
		cb_transformation.reset(new ConstantBuffer<VS_Property>(device));
		cb_color.reset(new ConstantBuffer<XMVECTOR>(device));
	}

	/*
	shader->SetPipline(dContext);
	for (auto& mark : marks) {

		graphic->SetRasterizerState();
		graphic->SetDepthStencilState();
		cb_transformation->VSSetData(dContext, &VS_Property(mark.second.geom, vpMat), 0);
		cb_color->PSSetData(dContext,&(mark.second.color), 0);
		mark.second.geom->Render(dContext);
	}*/
#pragma endregion

	#pragma region Lines

	static D3D11_BUFFER_DESC vb_desc;
	if (lineVB == nullptr)
	{
		vb_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vb_desc.ByteWidth = sizeof(Vertex) * 2;
		vb_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vb_desc.MiscFlags = 0;
		vb_desc.StructureByteStride = 0;
		vb_desc.Usage = D3D11_USAGE_DYNAMIC;

		r_assert(
			device->CreateBuffer(
				&vb_desc,
				nullptr,
				lineVB.GetAddressOf())
		);
	}

	shader->Apply(dContext);
	dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	for (auto& l : lines)
	{
		D3D11_MAPPED_SUBRESOURCE mapped;
		r_assert(
			dContext->Map(
				lineVB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped));
		Vertex* pVB = reinterpret_cast<Vertex*>(mapped.pData);
		pVB[0].pos = l.second.p1;
		pVB[1].pos = l.second.p2;
		dContext->Unmap(lineVB.Get(), 0);
		
		cb_transformation->VSSetData(dContext, &VS_Property(vpMat), 0);
		cb_color->PSSetData(dContext, &(l.second.color), 0);

		UINT stride = sizeof(Vertex);
		UINT offset = 0;

		dContext->IASetVertexBuffers(0, 1, lineVB.GetAddressOf(), &stride, &offset);
		dContext->Draw(2, 0);
	}

	if(gridVB != nullptr)
	{
		cb_transformation->VSSetData(dContext, &VS_Property(vpMat), 0);
		cb_color->PSSetData(dContext, &((XMVECTOR)Colors::Red), 0);
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		dContext->IASetVertexBuffers(0, 1, originVB.GetAddressOf(), &stride, &offset);
		dContext->Draw(2, 0);

		cb_transformation->VSSetData(dContext, &VS_Property(vpMat), 0);
		cb_color->PSSetData(dContext, &((XMVECTOR)Colors::Green), 0);
		dContext->IASetVertexBuffers(0, 1, originVB.GetAddressOf(), &stride, &offset);
		dContext->Draw(2, 2);

		cb_transformation->VSSetData(dContext, &VS_Property(vpMat), 0);
		cb_color->PSSetData(dContext, &((XMVECTOR)Colors::Blue), 0);
		dContext->IASetVertexBuffers(0, 1, originVB.GetAddressOf(), &stride, &offset);
		dContext->Draw(2, 4);

		cb_transformation->VSSetData(dContext, &VS_Property(vpMat), 0);
		cb_color->PSSetData(dContext, &((XMVECTOR)Colors::Gray), 0);
		dContext->IASetVertexBuffers(0, 1, gridVB.GetAddressOf(), &stride, &offset);
		dContext->Draw(gridVerticeCount, 0);
	}

#pragma endregion


	#pragma region Texts

	if (spriteBatch == nullptr)
	{
		spriteBatch = std::make_unique<DirectX::SpriteBatch>(dContext);
		spriteFont = std::make_unique<DirectX::SpriteFont>(device, L"Data\\Font\\Font.spritefont");
	}
	spriteBatch->Begin();
	for (auto& text : texts)
	{
		XMFLOAT4 textPos = XMFLOAT4(text.pos.x, text.pos.y, text.pos.z,1);
		if (text.is3D)
		{
			textPos = textPos * vpMat;
			textPos /= textPos.w;
			textPos = textPos * textMat;

			if (textPos.z < 0)
			{
				textPos.x = 99999;
				textPos.y = 99999;
			}
		}

		spriteFont->DrawString(
			spriteBatch.get(),
			text.tex.c_str(),
			XMFLOAT2(textPos.x, textPos.y),
			text.color,
			0.0f,
			XMFLOAT2(0, 0),
			XMFLOAT2(text.scale, text.scale)
		);
	}
	texts.clear();
	spriteBatch->End();
#pragma endregion
}
