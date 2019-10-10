#pragma once
#include "DX_info.h"
#include "Game_info.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <unordered_set>
#include <unordered_map>
#include "Singleton.h"

class Transform;
class Shape;
class VShader;
class HShader;
class DShader;
class GShader;
class PShader;
class Buffer;
class DepthStencilState;
class BlendState;
class RasterizerState;
class Camera;
class DebuggingScene;
struct SHADER_STD_TRANSF;

class Debugging : public Singleton<Debugging>
{
public:
	friend class DebuggingScene;

	void Draw(const std::string tex, const float x, const float y,  const XMVECTORF32 _color = Colors::White,  const float _scale = 1.5f);
	void Draw(const int tex, const float x, const float y, const XMVECTORF32 _color = Colors::White, const float _scale = 1.5f);
	void Draw(float tex, const float x, const float y, XMVECTORF32 _color = Colors::White, float _scale = 1.5f);
	void Draw(std::string title, XMFLOAT3 v, const float x, const float y, XMVECTORF32 _color = Colors::White, float _scale = 1.0f);
	void Draw3D(const std::string tex, const XMFLOAT3 _pos, const XMVECTORF32 _color = Colors::White, const float _scale = 1.5f);
	void Draw3D(const int tex, const XMFLOAT3 _pos, const XMVECTORF32 _color = Colors::White, const float _scale = 1.5f);
	void Draw3D(float tex, const XMFLOAT3 _pos, XMVECTORF32 _color = Colors::White, float _scale = 1.5f);
	void Draw3D(std::string title, XMFLOAT3 v,  const XMFLOAT3 _pos, XMVECTORF32 _color = Colors::White, float _scale = 1.0f);
	void Mark(const UINT key, XMFLOAT3 pos, float radius = 1.0f, XMVECTORF32 color = Colors::Red);
	void PtLine(const UINT key, XMFLOAT3 p1, XMFLOAT3 p2, XMVECTORF32 color = Colors::White);
	void DirLine(const UINT key, XMFLOAT3 p1, XMFLOAT3 dir, float dist, XMVECTORF32 color = Colors::White);

	void EnableGrid(float interval, int num=100);
	void DisableGrid();

private:
	void Update(const Camera* camera);
	void Render();

	friend class Singleton<Debugging>;
	Debugging();
	~Debugging();

	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
	std::unique_ptr<DirectX::SpriteFont> spriteFont;

	struct ScreenTextInfo {
		std::string tex;
		XMFLOAT3 pos;
		XMVECTOR color;
		float scale;
		bool is3D;

		ScreenTextInfo() {}
	};
	std::vector<ScreenTextInfo> texts;

	struct MarkInfo {
		Transform* transform;
		Shape* geom;
		XMVECTOR color;

		MarkInfo() {}
		MarkInfo(Transform* transform, Shape* geom, XMVECTOR color) 
			:transform(transform), geom(geom), color(color)
		{
		}
	};
	std::unordered_map<UINT, MarkInfo> marks;

	struct LineInfo {
		XMFLOAT3 p1, p2;
		XMVECTOR color;

		LineInfo() {}
		LineInfo(const XMFLOAT3 _p1, const XMFLOAT3 _p2, const XMVECTORF32 _c) {
			p1 = _p1;
			p2 = _p2;
			color = _c;
		}
	};
	std::unordered_map<UINT,LineInfo> lines;
	Buffer* lineVB=nullptr;
	Buffer* gridVB=nullptr;
	Buffer* originVB=nullptr;
	UINT gridVerticeCount;
	VShader* vs;
	HShader* hs;
	DShader* ds;
	GShader* gs;
	PShader* ps;
	DepthStencilState* dsState;
	BlendState* blendState;
	RasterizerState* rsState;

	float gridInterval;

	XMMATRIX vp_mat;

};

