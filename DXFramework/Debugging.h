#pragma once

#include "Singleton.h"


#include "_Packages\directxtk_desktop_2015.2019.12.17.1\include\SpriteBatch.h"
#include "_Packages\directxtk_desktop_2015.2019.12.17.1\include\SpriteFont.h"

class Transform;
class Mesh;
class VShader;
class PShader;
class Buffer;
class DepthStencilState;
class BlendState;
class RasterizerState;
class Camera;
class DebuggingScene;
class IDebug;
struct SHADER_STD_TRANSF;

#define MARK_MAX 220

class Debugging : public Singleton<Debugging>
{
public:
	friend class DebuggingScene;

	void Draw(const std::string tex, const float x, const float y, const XMVECTORF32 _color = Colors::White, const float _scale = 1.5f);
	void Draw(const int tex, const float x, const float y, const XMVECTORF32 _color = Colors::White, const float _scale = 1.5f);
	void Draw(float tex, const float x, const float y, XMVECTORF32 _color = Colors::White, float _scale = 1.5f);
	void Draw(std::string title, float v, const float x, const float y, XMVECTORF32 _color = Colors::White, float _scale = 1.0f);
	void Draw(std::string title, XMFLOAT3 v, const float x, const float y, XMVECTORF32 _color = Colors::White, float _scale = 1.0f);
	void Draw3D(const std::string tex, const XMFLOAT3 _pos, const XMVECTORF32 _color = Colors::White, const float _scale = 1.5f);
	void Draw3D(const int tex, const XMFLOAT3 _pos, const XMVECTORF32 _color = Colors::White, const float _scale = 1.5f);
	void Draw3D(float tex, const XMFLOAT3 _pos, XMVECTORF32 _color = Colors::White, float _scale = 1.5f);
	void Draw3D(std::string title, XMFLOAT3 v, const XMFLOAT3 _pos, XMVECTORF32 _color = Colors::White, float _scale = 1.0f);
	void Mark(XMFLOAT3 pos, float radius = 1.0f, XMVECTORF32 color = Colors::Red);
	void MarkCube(XMFLOAT3 center, XMFLOAT3 forward, XMFLOAT3 up, XMFLOAT3 extent);
	void PtLine(XMFLOAT3 p1, XMFLOAT3 p2, XMVECTORF32 color = Colors::White);
	void DirLine(XMFLOAT3 p1, XMFLOAT3 dir, float dist, XMVECTORF32 color = Colors::White);

	void EnableGrid(float interval, int num = 100);
	void DisableGrid();

	void Visualize(IDebug* obj);

	~Debugging();

	void Enabled(bool b) { enabled = b; }
	void SetCamera(const Camera* cam);

private:
	const Camera* debugCam=nullptr;
	void Update(float spf);
	void Render();

	bool enabled = true;

	friend class Singleton<Debugging>;
	Debugging();

	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
	std::unique_ptr<DirectX::SpriteFont> spriteFont;

	struct ScreenTextInfo {
		std::string tex;
		XMFLOAT3 pos;
		XMVECTOR color;
		float scale;
		bool is3D;

		ScreenTextInfo() :tex(""), pos(XMFLOAT3(0, 0, 0)), color(Colors::White), scale(1), is3D(false) {}
	};
	std::vector<ScreenTextInfo> texts;

	struct MarkInfo {
		bool isDraw;
		XMFLOAT3 pos;
		float rad;
		XMVECTOR color;

		MarkInfo() :isDraw(false), pos(XMFLOAT3(0, 0, 0)), rad(1), color(Colors::Red) {}
	};
	UINT curMarkIdx = 0;
	Transform* markTransform;
	Mesh* markShape;
	MarkInfo marks[MARK_MAX];

	struct LineInfo {
		XMFLOAT3 p1, p2;
		XMVECTOR color;

		LineInfo() :p1(XMFLOAT3(0, 0, 0)), p2(XMFLOAT3(0, 0, 0)), color(Colors::White) {}
		LineInfo(const XMFLOAT3 _p1, const XMFLOAT3 _p2, const XMVECTORF32 _c) {
			p1 = _p1;
			p2 = _p2;
			color = _c;
		}
	};
	std::vector<LineInfo> lines;
	Buffer* lineVB = nullptr;
	Buffer* gridVB = nullptr;
	Buffer* originVB = nullptr;
	UINT gridVerticeCount;
	VShader* markVS;
	PShader* markPS;
	DepthStencilState* dsState;
	BlendState* blendState;
	RasterizerState* rsState;

	float gridInterval;

	std::unordered_set<IDebug*> debugObjs;
};