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
#include "Keyboard.h"
#include "Mouse.h"

#include "Cube.h"
#include "Sphere.h"

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
		Transform* transform = new Transform();
		Sphere* newGeom = new Sphere(2);
		transform->SetTranslation(pos);
		transform->SetScale(radius, radius, radius);
		
		marks.insert(std::pair<UINT, MarkInfo>(key, MarkInfo(transform,newGeom, color)));
	}
	else {
		marks[key].transform->SetTranslation(pos);
		marks[key].transform->SetScale(radius, radius, radius);
		marks[key].color = color;
	}
}

void Debugging::PtLine(XMFLOAT3 p1, XMFLOAT3 p2, XMVECTORF32 color)
{
	lines.push_back(LineInfo(p1,p2,color));
}

void Debugging::DirLine(XMFLOAT3 p1, XMFLOAT3 dir, float dist, XMVECTORF32 color)
{
	XMFLOAT3 p2 = p1 + dir * dist;

	PtLine(p1, p2, color);
}

void Debugging::EnableGrid(float interval, int num)
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
	originVB_desc.ByteWidth = sizeof(gridVertice[0])*originVertice.size();
	originVB_desc.CPUAccessFlags = 0;
	originVB_desc.MiscFlags = 0;
	originVB_desc.StructureByteStride = 0;
	originVB_desc.Usage = D3D11_USAGE_IMMUTABLE;
	if (originVB)
		delete originVB;
	originVB = new Buffer(&originVB_desc, originVertice.data());

	D3D11_BUFFER_DESC gridVB_desc;
	gridVB_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	gridVB_desc.ByteWidth = sizeof(gridVertice[0])*gridVertice.size();
	gridVB_desc.CPUAccessFlags = 0;
	gridVB_desc.MiscFlags = 0;
	gridVB_desc.StructureByteStride = 0;
	gridVB_desc.Usage = D3D11_USAGE_IMMUTABLE;
	if (gridVB)
		delete gridVB;
	gridVB = new Buffer(&gridVB_desc, gridVertice.data());
}

void Debugging::DisableGrid()
{
	delete gridVB;
	gridVB = nullptr;
}


void Debugging::CameraMove(float spf) {

	XMFLOAT3 newPos = testCamera->GetPos();
	XMFLOAT3 right = testCamera->GetRight();
	XMFLOAT3 forward = testCamera->GetForward();
	const float speed = 50;
	if (Keyboard::IsPressing('A')) {

		newPos += -right * speed * spf;
	}
	else if (Keyboard::IsPressing('D')) {

		newPos += right * speed * spf;
	}
	if (Keyboard::IsPressing('S')) {

		newPos += -forward * speed * spf;
	}
	else if (Keyboard::IsPressing('W')) {

		newPos += forward * speed * spf;
	}
	static float angleX = 0;
	static float angleY = 0;
	static XMFLOAT2 prevMousePt;
	const float angleSpeed = 3.141592f * 0.2f;
	if (Mouse::Instance()->IsRightDown())
	{
		angleY += angleSpeed * spf * (Mouse::Instance()->X() - prevMousePt.x);
		angleX += angleSpeed * spf * (Mouse::Instance()->Y() - prevMousePt.y);
	}
	prevMousePt.x = Mouse::Instance()->X();
	prevMousePt.y = Mouse::Instance()->Y();
	const XMMATRIX rotMat = XMMatrixRotationX(angleX) * XMMatrixRotationY(angleY);
	testCamera->SetPos(newPos);
	XMFLOAT3 f = FORWARD * rotMat;
	XMFLOAT3 u = UP * rotMat;
	testCamera->SetRot(f, u);
}
void Debugging::Update(const Camera* camera, float spf)
{
	vp_mat = camera->VMat() * camera->ProjMat(Z_ORDER_STANDARD);
	CameraMove(spf);
}

void Debugging::Render()
{
	hs->Apply();
	ds->Apply();
	gs->Apply();
	blendState->Apply();
	dsState->Apply();
	rsState->Apply();

	#pragma region Marks

	for (auto& mark : marks) {

		vs->WriteCB(0,&(mark.second.transform->WorldMatrix() * vp_mat));
		ps->WriteCB(0,&(mark.second.color));
		vs->Apply();
		ps->Apply();
		mark.second.geom->Apply();
	}
#pragma endregion

	#pragma region Lines

	

	DX_DContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	for (auto& l : lines)
	{
		D3D11_MAPPED_SUBRESOURCE mapped;
		r_assert(
			DX_DContext->Map(
				lineVB->Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped));
		XMFLOAT3* pVB = reinterpret_cast<XMFLOAT3*>(mapped.pData);
		pVB[0] = l.p1;
		pVB[1] = l.p2;
		DX_DContext->Unmap(lineVB->Get(), 0);
		
		vs->WriteCB(0,&vp_mat);
		ps->WriteCB(0,&(l.color));
		vs->Apply();
		ps->Apply();

		UINT stride = sizeof(XMFLOAT3);
		UINT offset = 0;
		DX_DContext->IASetVertexBuffers(0, 1, lineVB->GetAddress(), &stride, &offset);
		DX_DContext->Draw(2, 0);
	}
	lines.clear();

	if (gridVB)
	{
		vs->WriteCB(0,&vp_mat);
		ps->WriteCB(0,(void*)(&(Colors::Red)));
		vs->Apply();
		ps->Apply();
		UINT stride = sizeof(XMFLOAT3);
		UINT offset = 0;
		DX_DContext->IASetVertexBuffers(0, 1, originVB->GetAddress(), &stride, &offset);
		DX_DContext->Draw(2, 0);

		vs->WriteCB(0,&vp_mat);
		ps->WriteCB(0,(void*)(&(Colors::Green)));
		vs->Apply();
		ps->Apply();
		DX_DContext->IASetVertexBuffers(0, 1, originVB->GetAddress(), &stride, &offset);
		DX_DContext->Draw(2, 2);

		vs->WriteCB(0,&vp_mat);
		ps->WriteCB(0,(void*)(&(Colors::Blue)));
		vs->Apply();
		ps->Apply();
		DX_DContext->IASetVertexBuffers(0, 1, originVB->GetAddress(), &stride, &offset);
		DX_DContext->Draw(2, 4);

		vs->WriteCB(0,&vp_mat);
		ps->WriteCB(0,(void*)(&(Colors::Gray)));
		vs->Apply();
		ps->Apply();
		DX_DContext->IASetVertexBuffers(0, 1, gridVB->GetAddress(), &stride, &offset);
		DX_DContext->Draw(gridVerticeCount, 0);
	}

#pragma endregion

	#pragma region Texts

	
	spriteBatch->Begin();
	for (auto& text : texts)
	{
		XMFLOAT4 textPos = XMFLOAT4(text.pos.x, text.pos.y, text.pos.z,1);
		if (text.is3D)
		{
			textPos = textPos * vp_mat;
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


Debugging::Debugging()
{
	testCamera = new Camera("DebugCamera", FRAME_KIND_PERSPECTIVE, SCREEN_WIDTH, SCREEN_HEIGHT, 0.1f, 1000.0f, XM_PIDIV2, 1);
	testCamera->SetPos(XMFLOAT3(0, 10, -30));
	testCamera->SetRot(FORWARD, UP);
	testCamera->SetMain();

	vs = new VShader("MarkVS.cso", 
		simple_ILayouts,
		ARRAYSIZE(simple_ILayouts));
	hs = new HShader();
	ds = new DShader();
	gs = new GShader();
	ps = new PShader("MarkPS.cso");
	vs->AddCB(0, 1, sizeof(XMMATRIX));
	ps->AddCB(0, 1, sizeof(XMVECTOR));

	blendState = new BlendState(nullptr);
	dsState = new DepthStencilState(nullptr);
	rsState = new RasterizerState(nullptr);

	spriteBatch = std::make_unique<DirectX::SpriteBatch>(DX_DContext);
	spriteFont = std::make_unique<DirectX::SpriteFont>(DX_Device, L"Data\\Font\\Font.spritefont");

	D3D11_BUFFER_DESC vb_desc;
	vb_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vb_desc.ByteWidth = sizeof(XMFLOAT3) * 2;
	vb_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vb_desc.MiscFlags = 0;
	vb_desc.StructureByteStride = 0;
	vb_desc.Usage = D3D11_USAGE_DYNAMIC;
	lineVB = new Buffer(&vb_desc, nullptr);

	
}

Debugging::~Debugging()
{
	delete vs;
	delete gs;
	delete ps;
	delete blendState;
	delete dsState;
	delete rsState;

	delete lineVB;
	delete gridVB;
}
