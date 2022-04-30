
#include "pch.h"

#include "Debugging.h"
#include <string>
#include "Shader.h"
#include "Graphic.h"
#include "ShaderFormat.h"
#include "Buffer.h"
#include "BlendState.h"
#include "DepthStencilState.h"
#include "RasterizerState.h"
#include "Transform.h"
#include "Camera.h"

#include "Network.h"
#include "LayerMask.h"
#include "SphereMesh.h"
#include "ShaderReg.h"
#include "Vertex.h"

using namespace DX;

Debugging::Debugging(ID3D11Device* device, ID3D11DeviceContext* dContext)
{
	auto descs = D3DLayout_Simple().GetLayout();

	markVS = new VShader(device, "UnlitVS.cso",
		descs.data(),
		descs.size());
	markPS = new PShader(device, "UnlitPS.cso");
	markVS->AddCB(device, 0, 1, sizeof(XMMATRIX));
	markPS->AddCB(device, SHADER_REG_CB_COLOR, 1, sizeof(XMVECTOR));

	blendState = new BlendState(device, nullptr);
	dsState = new DepthStencilState(device, nullptr);
	D3D11_RASTERIZER_DESC wrs_desc;
	ZeroMemory(&wrs_desc, sizeof(D3D11_RASTERIZER_DESC));
	wrs_desc.FillMode = D3D11_FILL_WIREFRAME;
	wrs_desc.CullMode = D3D11_CULL_BACK;
	rsState = new RasterizerState(device , &wrs_desc);

	spriteBatch = std::make_unique<DirectX::SpriteBatch>(dContext);
	std::string fontPath = __FILE__"\\..\\Font\\Font.spritefont";
	spriteFont = std::make_unique<DirectX::SpriteFont>(device, std::wstring(fontPath.begin(), fontPath.end()).c_str());

	D3D11_BUFFER_DESC vb_desc;
	vb_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vb_desc.ByteWidth = sizeof(XMFLOAT3) * 2;
	vb_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vb_desc.MiscFlags = 0;
	vb_desc.StructureByteStride = 0;
	vb_desc.Usage = D3D11_USAGE_DYNAMIC;
	lineVB = new Buffer(device ,&vb_desc, nullptr);

	VertexLayout layout =  D3DLayout_Simple();
	m_markShape = new SphereMesh(device, 2, &layout);
	m_markTransform = new Transform();
}

Debugging::~Debugging()
{
	delete markVS;
	delete markPS;
	delete blendState;
	delete dsState;
	delete rsState;
	delete m_markShape;
	delete m_markTransform;

	delete lineVB;
	delete gridVB;
	delete originVB;
}

void Debugging::SetCamera(const Camera * cam)
{
	debugCam = cam;
}


void Debugging::Draw(const std::string tex, const float x, const float y, const XMVECTORF32 _color, const float _scale)
{
#ifdef _DEBUG
	ScreenTextInfo newText;
	newText.tex = tex;
	newText.pos = XMFLOAT3(x, y, 1);
	newText.color = _color;
	newText.scale = _scale;
	newText.is3D = false;

	texts.push_back(newText);
#endif
}
void Debugging::Draw(const int tex, const float x, const float y, const XMVECTORF32 _color, const float _scale)
{
	Draw(std::to_string(tex), x, y, _color, _scale);
}
void Debugging::Draw(float tex, const float x, const float y, XMVECTORF32 _color, float _scale)
{
	Draw(std::to_string(tex), x, y, _color, _scale);
}
void Debugging::Draw(std::string title, float v, const float x, const float y, XMVECTORF32 _color, float _scale)
{
	Draw(title + std::to_string(v), x, y, _color, _scale);
}
void Debugging::Draw(std::string title, XMFLOAT3 v, const float x, const float y, XMVECTORF32 _color, float _scale)
{
	Draw(title + "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + ")", x, y, _color, _scale);
}
void Debugging::Draw3D(const std::string tex, const XMFLOAT3 _pos, const XMVECTORF32 _color, const float _scale)
{
#ifdef _DEBUG
	ScreenTextInfo newText;
	newText.tex = tex;
	newText.pos = _pos;
	newText.color = _color;
	newText.scale = _scale;
	newText.is3D = true;

	texts.push_back(newText);
#endif
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
void Debugging::Mark(XMFLOAT3 pos, float radius, XMVECTORF32 color)
{
#ifdef _DEBUG
	for (int i = 0; i < MARK_MAX; ++i)
	{
		if (m_curMarkIdx >= MARK_MAX)
			m_curMarkIdx = 0;

		if (m_marks[m_curMarkIdx].isDraw == false)
		{
			m_marks[m_curMarkIdx].isDraw = true;
			m_marks[m_curMarkIdx].pos = pos;
			m_marks[m_curMarkIdx].rad = radius;
			m_marks[m_curMarkIdx].color = color;
			m_curMarkIdx++;
			return;
		}
		else {
			m_curMarkIdx++;
		}
	}

	assert("debug mark capacity overflow");
#endif
}

void Debugging::MarkCube(XMFLOAT3 center, XMFLOAT3 forward, XMFLOAT3 up, XMFLOAT3 extent)
{
	XMFLOAT3 right = Cross(up, forward);

	PtLine(center + right * extent.x + up * extent.y + forward * extent.z, center - right * extent.x + up * extent.y + forward * extent.z);
	PtLine(center + right * extent.x + up * extent.y + forward * extent.z, center + right * extent.x + up * extent.y - forward * extent.z);
	PtLine(center + right * extent.x + up * extent.y - forward * extent.z, center - right * extent.x + up * extent.y - forward * extent.z);
	PtLine(center - right * extent.x + up * extent.y - forward * extent.z, center - right * extent.x + up * extent.y + forward * extent.z);

	PtLine(center + right * extent.x - up * extent.y + forward * extent.z, center + right * extent.x + up * extent.y + forward * extent.z);
	PtLine(center - right * extent.x - up * extent.y + forward * extent.z, center - right * extent.x + up * extent.y + forward * extent.z);
	PtLine(center + right * extent.x - up * extent.y - forward * extent.z, center + right * extent.x + up * extent.y - forward * extent.z);
	PtLine(center - right * extent.x - up * extent.y - forward * extent.z, center - right * extent.x + up * extent.y - forward * extent.z);

	PtLine(center + right * extent.x - up * extent.y + forward * extent.z, center - right * extent.x - up * extent.y + forward * extent.z);
	PtLine(center + right * extent.x - up * extent.y + forward * extent.z, center + right * extent.x - up * extent.y - forward * extent.z);
	PtLine(center + right * extent.x - up * extent.y - forward * extent.z, center - right * extent.x - up * extent.y - forward * extent.z);
	PtLine(center - right * extent.x - up * extent.y - forward * extent.z, center - right * extent.x - up * extent.y + forward * extent.z);
}

void Debugging::PtLine(XMFLOAT3 p1, XMFLOAT3 p2, XMVECTORF32 color)
{
	lines.push_back(LineInfo(p1, p2, color));
}

void Debugging::DirLine(XMFLOAT3 p1, XMFLOAT3 dir, float dist, XMVECTORF32 color)
{
	XMFLOAT3 p2 = p1 + dir * dist;

	PtLine(p1, p2, color);
}

void Debugging::EnableGrid(ID3D11Device* device, float interval, int num)
{
	assert(num % 2 == 0);

	const float LENGTH = interval * num;
	const float HLENGTH = LENGTH * 0.5f;
	std::vector<XMFLOAT3> originVertice;
	std::vector<XMFLOAT3> gridVertice;
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
	originVB_desc.ByteWidth = sizeof(gridVertice[0]) * originVertice.size();
	originVB_desc.CPUAccessFlags = 0;
	originVB_desc.MiscFlags = 0;
	originVB_desc.StructureByteStride = 0;
	originVB_desc.Usage = D3D11_USAGE_IMMUTABLE;
	if (originVB)
		delete originVB;
	originVB = new Buffer(device, &originVB_desc, originVertice.data());

	D3D11_BUFFER_DESC gridVB_desc;
	gridVB_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	gridVB_desc.ByteWidth = sizeof(gridVertice[0]) * gridVertice.size();
	gridVB_desc.CPUAccessFlags = 0;
	gridVB_desc.MiscFlags = 0;
	gridVB_desc.StructureByteStride = 0;
	gridVB_desc.Usage = D3D11_USAGE_IMMUTABLE;
	if (gridVB)
		delete gridVB;
	gridVB = new Buffer(device, &gridVB_desc, gridVertice.data());
}

void Debugging::DisableGrid()
{
	delete gridVB;
	gridVB = nullptr;
}

void Debugging::Visualize(IDebug* obj)
{
#ifdef _DEBUG
	assert(debugObjs.find(obj) == debugObjs.end());

	debugObjs.insert(obj);
#endif
}

void Debugging::Update(float spf)
{
	if(!enabled)
		return;

	for (IDebug* obj : debugObjs)
	{
		obj->Visualize();
	}
}

void Debugging::Render(ID3D11DeviceContext* dContext, int iScnWidth, int iScnHeight)
{
	if (!enabled)
		return;

	dContext->HSSetShader(nullptr, nullptr, 0);
	dContext->DSSetShader(nullptr, nullptr, 0);
	dContext->GSSetShader(nullptr, nullptr, 0);
	blendState->Apply(dContext);
	dsState->Apply(dContext);
	rsState->Apply(dContext);

#pragma region Marks

	for (int i = 0; i < MARK_MAX; ++i) {

		if (!m_marks[i].isDraw)
			continue;

		XMMATRIX vp = debugCam->VMat() * debugCam->ProjMat();

		m_markTransform->SetTranslation(m_marks[i].pos);
		m_markTransform->SetScale(m_marks[i].rad);
		auto mvp = m_markTransform->WorldMatrix() * vp;
		markVS->WriteCB(dContext, 0, &mvp);
		markPS->WriteCB(dContext,SHADER_REG_CB_COLOR, &(m_marks[i].color));
		markVS->Apply(dContext);
		markPS->Apply(dContext);
		m_markShape->Apply(dContext);
		m_marks[i].isDraw = false;
	}
#pragma endregion

#pragma region Lines

	dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	for (auto& l : lines)
	{
		XMMATRIX vp = debugCam->VMat() * debugCam->ProjMat();

		D3D11_MAPPED_SUBRESOURCE mapped;
		ZeroMemory(&mapped, sizeof(D3D11_MAPPED_SUBRESOURCE));
		HRESULT hr = dContext->Map(
				lineVB->Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
		r_assert(hr);
		XMFLOAT3* pVB = reinterpret_cast<XMFLOAT3*>(mapped.pData);
		pVB[0] = l.p1;
		pVB[1] = l.p2;
		dContext->Unmap(lineVB->Get(), 0);

		markVS->WriteCB(dContext,0, &vp);
		markPS->WriteCB(dContext,SHADER_REG_CB_COLOR, &(l.color));
		markVS->Apply(dContext);
		markPS->Apply(dContext);

		UINT stride = sizeof(XMFLOAT3);
		UINT offset = 0;
		dContext->IASetVertexBuffers(0, 1, lineVB->GetAddress(), &stride, &offset);
		dContext->Draw(2, 0);
	}
	lines.clear();

	if (gridVB)
	{
		XMMATRIX vp = debugCam->VMat() * debugCam->ProjMat();

		markVS->WriteCB(dContext, 0, &vp);
		markPS->WriteCB(dContext, SHADER_REG_CB_COLOR, (void*)(&(Colors::Red)));
		markVS->Apply(dContext);
		markPS->Apply(dContext);
		UINT stride = sizeof(XMFLOAT3);
		UINT offset = 0;
		dContext->IASetVertexBuffers(0, 1, originVB->GetAddress(), &stride, &offset);
		dContext->Draw(2, 0);

		markVS->WriteCB(dContext, 0, &vp);
		markPS->WriteCB(dContext, SHADER_REG_CB_COLOR, (void*)(&(Colors::Green)));
		markVS->Apply(dContext);
		markPS->Apply(dContext);
		dContext->IASetVertexBuffers(0, 1, originVB->GetAddress(), &stride, &offset);
		dContext->Draw(2, 2);

		markVS->WriteCB(dContext, 0, &vp);
		markPS->WriteCB(dContext, SHADER_REG_CB_COLOR, (void*)(&(Colors::Blue)));
		markVS->Apply(dContext);
		markPS->Apply(dContext);
		dContext->IASetVertexBuffers(0, 1, originVB->GetAddress(), &stride, &offset);
		dContext->Draw(2, 4);

		markVS->WriteCB(dContext, 0, &vp);
		markPS->WriteCB(dContext,SHADER_REG_CB_COLOR, (void*)(&(Colors::Gray)));
		markVS->Apply(dContext);
		markPS->Apply(dContext);
		dContext->IASetVertexBuffers(0, 1, gridVB->GetAddress(), &stride, &offset);
		dContext->Draw(gridVerticeCount, 0);
	}

#pragma endregion

#pragma region Texts


	const XMMATRIX textMat = XMMATRIX(
		iScnWidth / 2.0f, 0, 0, 0,
		0, -iScnHeight / 2.0f, 0, 0,
		0, 0, 1, 0,
		iScnWidth / 2.0f, iScnHeight / 2.0f, 0, 1);

	spriteBatch->Begin();
	for (auto& text : texts)
	{
		XMFLOAT4 textPos = XMFLOAT4(text.pos.x, text.pos.y, text.pos.z, 1);
		if (text.is3D)
		{
			XMMATRIX vp = debugCam->VMat() * debugCam->ProjMat();

			textPos = Multiply(textPos, vp);
			textPos /= textPos.w;
			textPos = Multiply(textPos, textMat);

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
