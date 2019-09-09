#pragma once
#include "DX_info.h"
#include "Game_info.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <unordered_set>
#include <unordered_map>
#include "Resource.h"

class Shape;
class IGraphic;
class VPShader;
class Camera;
struct VS_Property;

class Debugging
{
public:

	static void Draw(const std::string tex, const float x, const float y,  const XMVECTORF32 _color = Colors::White,  const float _scale = 1.5f);
	static void Draw(const int tex, const float x, const float y, const XMVECTORF32 _color = Colors::White, const float _scale = 1.5f);
	static void Draw(float tex, const float x, const float y, XMVECTORF32 _color = Colors::White, float _scale = 1.5f);
	static void Draw(std::string title, XMFLOAT3 v, const float x, const float y, XMVECTORF32 _color = Colors::White, float _scale = 1.0f);
	static void Draw3D(const std::string tex, const XMFLOAT3 _pos, const XMVECTORF32 _color = Colors::White, const float _scale = 1.5f);
	static void Draw3D(const int tex, const XMFLOAT3 _pos, const XMVECTORF32 _color = Colors::White, const float _scale = 1.5f);
	static void Draw3D(float tex, const XMFLOAT3 _pos, XMVECTORF32 _color = Colors::White, float _scale = 1.5f);
	static void Draw3D(std::string title, XMFLOAT3 v,  const XMFLOAT3 _pos, XMVECTORF32 _color = Colors::White, float _scale = 1.0f);
	static void Mark(const UINT key, XMFLOAT3 pos, float radius = 1.0f, XMVECTORF32 color = Colors::Red);
	static void Line(const UINT key, XMFLOAT3 p1, XMFLOAT3 p2, XMVECTORF32 color = Colors::White);

	static void EnableGrid(float interval, int num=100);
	static void DisableGrid();

	static void Render(Camera* camera);

	//edit - move into private
private:
	Debugging() {};

	static std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
	static std::unique_ptr<DirectX::SpriteFont> spriteFont;

	struct ScreenTextInfo {
		std::string tex;
		XMFLOAT3 pos;
		XMVECTOR color;
		float scale;
		bool is3D;

		ScreenTextInfo() {}
	};
	static std::vector<ScreenTextInfo> texts;

	struct MarkInfo {
		Shape* geom;
		XMVECTOR color;

		MarkInfo() {}
		MarkInfo(Shape* _geom, XMVECTOR _color) {
			geom = _geom;
			color = _color;
		}
	};
	static std::unordered_map<UINT, MarkInfo> marks;

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
	static std::unordered_map<UINT,LineInfo> lines;
	static ComPtr<ID3D11Buffer> lineVB;
	static ComPtr<ID3D11Buffer> gridVB;
	static ComPtr<ID3D11Buffer> originVB;
	static UINT gridVerticeCount;
	static std::unique_ptr<VPShader> shader;
	static std::unique_ptr<ConstantBuffer<VS_Property>> cb_transformation;
	static std::unique_ptr<ConstantBuffer<XMVECTOR>> cb_color;
	static ComPtr<ID3D11BlendState> blendState;

	static float gridInterval;
};

