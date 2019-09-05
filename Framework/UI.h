#pragma once
#include <string>
#include <unordered_map>
#include "DX_info.h"
#include "Geometrics.h"
#include "Resource.h"

class Transform;
class Quad;
class VPShader;
struct VS_Property;
class UI
{
private:
	// screen coordinate
	UI(ID3D11Device* device, float canvasWidth, float canvasHeight, XMFLOAT2 pivot, float width, float height, float zDepth, ID3D11ShaderResourceView * srv, UINT maxSliceIdx, UINT slicePerSec);
	~UI();
	void Update(float spf);
	void Render(ID3D11DeviceContext* dContext, const XMMATRIX& vpMat);
	friend class UICanvas;

	Transform* transform;
	Quad* quad;
	VPShader* shader;
	ConstantBuffer<VS_Property>* cb_vs_property;
	ConstantBuffer<float>* cb_ps_sliceIdx;
	ID3D11BlendState* blendState;
	ID3D11ShaderResourceView *const srv;
	float curTime=0;
	const int maxSliceIdx;
	const float secPerSlice;
	int curSliceIdx = 0;
	ID3D11SamplerState* texSampState;

};

class Camera;
class IGraphic;
class UICanvas
{
public:
	UICanvas(ID3D11Device* device, float width, float height);
	~UICanvas();

	// screen coordinate
	void Add(ID3D11Device* device, std::string id, XMFLOAT2 pivot, float width, float height, float zDepth, ID3D11ShaderResourceView*const srv, UINT maxSliceIdx = 1, UINT slicePerSec = 1);
	void Remove(std::string id);

	void Update(float spf);
	void Render(IGraphic* graphic);

	const float totalWidth, totalHeight;

private:

	std::unordered_map<std::string, UI*> UIs;
	Camera* camera;
};

