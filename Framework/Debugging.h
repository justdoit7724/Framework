#pragma once
#include "DX_info.h"
#include "Game_info.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <unordered_set>
#include <unordered_map>
#include "Singleton.h"

class Shape;
class VPShader;
class Camera;
class Buffer;
class DepthStencilState;
class BlendState;
struct VS_Property;


class Debugging : public Singleton<Debugging>
{
public:

	void Draw(const std::string tex, const float x, const float y,  const XMVECTORF32 _color = Colors::White,  const float _scale = 1.5f);
	void Draw(const int tex, const float x, const float y, const XMVECTORF32 _color = Colors::White, const float _scale = 1.5f);
	void Draw(float tex, const float x, const float y, XMVECTORF32 _color = Colors::White, float _scale = 1.5f);
	void Draw(std::string title, XMFLOAT3 v, const float x, const float y, XMVECTORF32 _color = Colors::White, float _scale = 1.0f);
	void Draw3D(const std::string tex, const XMFLOAT3 _pos, const XMVECTORF32 _color = Colors::White, const float _scale = 1.5f);
	void Draw3D(const int tex, const XMFLOAT3 _pos, const XMVECTORF32 _color = Colors::White, const float _scale = 1.5f);
	void Draw3D(float tex, const XMFLOAT3 _pos, XMVECTORF32 _color = Colors::White, float _scale = 1.5f);
	void Draw3D(std::string title, XMFLOAT3 v,  const XMFLOAT3 _pos, XMVECTORF32 _color = Colors::White, float _scale = 1.0f);
	void Mark(const UINT key, XMFLOAT3 pos, float radius = 1.0f, XMVECTORF32 color = Colors::Red);
	void Line(const UINT key, XMFLOAT3 p1, XMFLOAT3 p2, XMVECTORF32 color = Colors::White);

	void EnableGrid(float interval, int num=100);
	void DisableGrid();

	void Render(Camera* camera);
private:

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

		ScreenTextInfo() {}
	};
	std::vector<ScreenTextInfo> texts;

	struct MarkInfo {
		Shape* geom;
		XMVECTOR color;

		MarkInfo() {}
		MarkInfo(Shape* _geom, XMVECTOR _color) {
			geom = _geom;
			color = _color;
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
	ComPtr<ID3D11Buffer> lineVB;
	ComPtr<ID3D11Buffer> gridVB;
	ComPtr<ID3D11Buffer> originVB;
	UINT gridVerticeCount;
	std::unique_ptr<VPShader> shader;
	std::unique_ptr<Buffer> cb_transformation;
	std::unique_ptr<Buffer> cb_color;
	std::unique_ptr<DepthStencilState> dsState;
	std::unique_ptr<BlendState> blendState;

	float gridInterval;
};

