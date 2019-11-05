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
#include "Network.h"

#include "Cube.h"
#include "Sphere.h"

Debugging::Debugging()
{
	debugCam = new Camera("DebugCamera", FRAME_KIND_PERSPECTIVE, SCREEN_WIDTH, SCREEN_HEIGHT, 0.1f, 1000.0f, XM_PIDIV2, 1);
	debugCam->transform->SetTranslation(XMFLOAT3(0, 10, -30));
	debugCam->transform->SetRot(FORWARD, UP);
	debugCam->SetMain();

	markVS = new VShader("MarkVS.cso",
		simple_ILayouts,
		ARRAYSIZE(simple_ILayouts));
	markPS = new PShader("MarkPS.cso");
	markVS->AddCB(0, 1, sizeof(XMMATRIX));
	markPS->AddCB(0, 1, sizeof(XMVECTOR));

	blendState = new BlendState(nullptr);
	dsState = new DepthStencilState(nullptr);
	D3D11_RASTERIZER_DESC wrs_desc;
	ZeroMemory(&wrs_desc, sizeof(D3D11_RASTERIZER_DESC));
	wrs_desc.FillMode = D3D11_FILL_WIREFRAME;
	wrs_desc.CullMode = D3D11_CULL_BACK;
	rsState = new RasterizerState(&wrs_desc);

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

	markShape = new Sphere(1);
	markTransform = new Transform();
}

Debugging::~Debugging()
{
	delete markVS;
	delete markPS;
	delete blendState;
	delete dsState;
	delete rsState;

	delete lineVB;
	delete gridVB;
}

const XMMATRIX textMat = XMMATRIX(
	SCREEN_WIDTH/2.0f, 0, 0, 0,
	0, -SCREEN_HEIGHT/2.0f, 0, 0,
	0, 0, 1, 0,
	SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0, 1);
void Debugging::Draw(const std::string tex, const float x, const float y, const XMVECTORF32 _color, const float _scale)
{
#ifdef _DEBUG
	ScreenTextInfo newText;
	newText.tex = tex;
	newText.pos = XMFLOAT3(x,y,1);
	newText.color = _color;
	newText.scale = _scale;
	newText.is3D = false;

	texts.push_back(newText);
#endif
}
void Debugging::Draw(const int tex, const float x, const float y, const XMVECTORF32 _color, const float _scale)
{
	Draw(std::to_string(tex), x,y, _color, _scale);
}
void Debugging::Draw(float tex, const float x, const float y, XMVECTORF32 _color, float _scale)
{
	Draw(std::to_string(tex), x,y, _color, _scale);
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
		if (curMarkIdx >= MARK_MAX)
			curMarkIdx = 0;

		if (marks[curMarkIdx].isDraw==false)
		{
			marks[curMarkIdx].isDraw = true;
			marks[curMarkIdx].pos = pos;
			marks[curMarkIdx].rad = radius;
			marks[curMarkIdx].color = color;
			curMarkIdx++;
			return;
		}
		else {
			curMarkIdx++;
		}
	}

	MB("debug mark capacity overflow");
#endif
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

void Debugging::Visualize(IDebug* obj)
{
#ifdef _DEBUG
	assert(debugObjs.find(obj) == debugObjs.end());
	
	debugObjs.insert(obj);
#endif
}

void Debugging::CameraMove(float spf) {

	XMFLOAT3 newPos = debugCam->transform->GetPos();
	XMFLOAT3 right = debugCam->transform->GetRight();
	XMFLOAT3 forward = debugCam->transform->GetForward();
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
	XMFLOAT2 mPt = Mouse::Instance()->Pos();
	if (Mouse::Instance()->RightState()==MOUSE_STATE_PRESSING)
	{
		angleY += angleSpeed * spf * (mPt.x - prevMousePt.x);
		angleX += angleSpeed * spf * (mPt.y - prevMousePt.y);
	}
	prevMousePt.x = mPt.x;
	prevMousePt.y = mPt.y;
	const XMMATRIX rotMat = XMMatrixRotationX(angleX) * XMMatrixRotationY(angleY);
	debugCam->transform->SetTranslation(newPos);
	XMFLOAT3 f = MultiplyDir(FORWARD, rotMat);
	XMFLOAT3 u = MultiplyDir(UP, rotMat);
	debugCam->transform->SetRot(f, u);
	debugCam->Update();
}
void Debugging::Update(float spf)
{
	CameraMove(spf);

	for (IDebug* obj : debugObjs)
	{
		obj->Visualize();
	}
}

void Debugging::Render()
{
	XMMATRIX vp_mat = debugCam->VMat() * debugCam->ProjMat(Z_ORDER_STANDARD);

	DX_DContext->HSSetShader(nullptr, nullptr, 0);
	DX_DContext->DSSetShader(nullptr, nullptr, 0);
	DX_DContext->GSSetShader(nullptr, nullptr, 0);
	blendState->Apply();
	dsState->Apply();
	rsState->Apply();

	#pragma region Marks

	for (int i = 0; i < MARK_MAX; ++i) {

		if (!marks[i].isDraw)
			continue;
		markTransform->SetTranslation(marks[i].pos);
		markTransform->SetScale(marks[i].rad);
		markVS->WriteCB(0,&(markTransform->WorldMatrix() * vp_mat));
		markPS->WriteCB(0,&(marks[i].color));
		markVS->Apply();
		markPS->Apply();
		markShape->Apply();
		marks[i].isDraw = false;
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
		
		markVS->WriteCB(0,&vp_mat);
		markPS->WriteCB(0,&(l.color));
		markVS->Apply();
		markPS->Apply();

		UINT stride = sizeof(XMFLOAT3);
		UINT offset = 0;
		DX_DContext->IASetVertexBuffers(0, 1, lineVB->GetAddress(), &stride, &offset);
		DX_DContext->Draw(2, 0);
	}
	lines.clear();

	if (gridVB)
	{
		markVS->WriteCB(0,&vp_mat);
		markPS->WriteCB(0,(void*)(&(Colors::Red)));
		markVS->Apply();
		markPS->Apply();
		UINT stride = sizeof(XMFLOAT3);
		UINT offset = 0;
		DX_DContext->IASetVertexBuffers(0, 1, originVB->GetAddress(), &stride, &offset);
		DX_DContext->Draw(2, 0);

		markVS->WriteCB(0,&vp_mat);
		markPS->WriteCB(0,(void*)(&(Colors::Green)));
		markVS->Apply();
		markPS->Apply();
		DX_DContext->IASetVertexBuffers(0, 1, originVB->GetAddress(), &stride, &offset);
		DX_DContext->Draw(2, 2);

		markVS->WriteCB(0,&vp_mat);
		markPS->WriteCB(0,(void*)(&(Colors::Blue)));
		markVS->Apply();
		markPS->Apply();
		DX_DContext->IASetVertexBuffers(0, 1, originVB->GetAddress(), &stride, &offset);
		DX_DContext->Draw(2, 4);

		markVS->WriteCB(0,&vp_mat);
		markPS->WriteCB(0,(void*)(&(Colors::Gray)));
		markVS->Apply();
		markPS->Apply();
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
			textPos = Multiply(textPos, vp_mat);
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
