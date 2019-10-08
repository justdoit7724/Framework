#pragma once
#include <string>
#include <unordered_map>
#include "DX_info.h"

class Transform;
class Quad;
class VShader;
class HShader;
class DShader;
class GShader;
class PShader;
class DepthStencilState;
class BlendState;

struct SHADER_TRANSFORMATION;
class UI
{
private:

	friend class UICanvas;

	// screen coordinate
	UI(float canvasWidth, float canvasHeight, XMFLOAT2 pivot, float width, float height, float zDepth, ID3D11ShaderResourceView * srv, UINT maxSliceIdx, UINT slicePerSec);
	// depth complexity
	~UI();
	void Update(float spf, const XMMATRIX& vpMat, const XMMATRIX& texMat);

	void Render();

	float curTime=0;
	const int maxSliceIdx;
	const float secPerSlice;
	int curSliceIdx = 0;
	ID3D11SamplerState* texSampState=nullptr;

public:
	Quad* quad;
	Transform* transform;
	VShader* vs;
	HShader* hs;
	DShader* ds;
	GShader* gs;
	PShader* ps;
	DepthStencilState* dsState;
	BlendState* blendState;
};

class Camera;
class UICanvas
{
public:
	UICanvas(float width, float height);
	~UICanvas();

	// screen coordinate
	void Add(std::string id, XMFLOAT2 pivot, float width, float height, float zDepth, ID3D11ShaderResourceView* srv, UINT maxSliceIdx = 1, UINT slicePerSec = 1);

	void Remove(std::string id);
	UI* Get(std::string id);

	void Update(float spf);
	void Render();

	const float totalWidth, totalHeight;

private:

	std::unordered_map<std::string, UI*> UIs;
	Camera* camera;
};

