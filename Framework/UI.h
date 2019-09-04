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
	UI(ID3D11Device* device, float canvasWidth, float canvasHeight, XMFLOAT2 pivot, float width, float height, float zDepth, ID3D11ShaderResourceView*const* texture);
	~UI();
	void Render(ID3D11DeviceContext* dContext, const XMMATRIX& vpMat);
	friend class UICanvas;

	Transform* transform;
	Quad* quad;
	VPShader* shader;
	ConstantBuffer<VS_Property>* cb_vs_property;
	ID3D11ShaderResourceView*const* texture;
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
	void Add(ID3D11Device* device, std::string id, XMFLOAT2 pivot, float width, float height, float zDepth, ID3D11ShaderResourceView*const* srv);
	void Remove(std::string id);
	void Render(IGraphic* graphic);

	const float totalWidth, totalHeight;

private:

	std::unordered_map<std::string, UI*> UIs;
	Camera* camera;
};

