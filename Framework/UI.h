#pragma once
#include <string>
#include <unordered_map>
#include "DX_info.h"

class Transform;
class Quad;
class Buffer;
class VPShader;
class DepthStencilState;
class BlendState;

struct VS_Property;
class UI
{
private:

	friend class UICanvas;

	// screen coordinate
	UI(float canvasWidth, float canvasHeight, XMFLOAT2 pivot, float width, float height, float zDepth, ID3D11ShaderResourceView * srv, UINT maxSliceIdx, UINT slicePerSec);
	~UI();
	void Update(float spf, const XMMATRIX& vpMat, const XMMATRIX& texMat);
	void Render();

	Transform* transform;
	Quad* quad;
	std::unique_ptr<Buffer> cb_vs_property;
	std::unique_ptr<Buffer> cb_ps_sliceIdx;
	ID3D11ShaderResourceView *const srv;
	float curTime=0;
	const int maxSliceIdx;
	const float secPerSlice;
	int curSliceIdx = 0;
	ID3D11SamplerState* texSampState;
	VPShader* shader;
	DepthStencilState* dsState;
	BlendState* blendState;

public:
	VPShader* GetShader(){return shader;}
	DepthStencilState* GetDSState(){return dsState;}
	BlendState* GetBlendState(){return blendState;}
};

class Camera;
class IGraphic;
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

