#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include "DX_info.h"
#include "ObserverDP.h"
#include "Geometrics.h"

class Transform;
class Camera;
class Quad;
class VShader;
class HShader;
class DShader;
class GShader;
class PShader;
class DepthStencilState;
class BlendState;

struct SHADER_STD_TRANSF;
class UI
{
public:
	UI(float canvasWidth, float canvasHeight, XMFLOAT2 pivot, float width, float height, float zDepth, ID3D11ShaderResourceView * srv);
	~UI();

protected:

	Quad* quad;
	Transform* transform;
	VShader* vs;
	PShader* ps;
	DepthStencilState* dsState;
	BlendState* blendState;
	ID3D11ShaderResourceView* srv;

	XMFLOAT2 size;

	friend class UICanvas;
	virtual void Update();

	virtual void Render(const Camera* camera)const;
};

class UIButton : public UI, public Subject
{
public:
	UIButton(float canvasWidth, float canvasHeight, XMFLOAT2 pivot, XMFLOAT2 size, ID3D11ShaderResourceView* idleSRV, ID3D11ShaderResourceView* hoverSRV, ID3D11ShaderResourceView* pressSRV);
	~UIButton();
private:
	friend class UICanvas;

	void Update() override ;

	void Render(const Camera* camera)const override;


	ID3D11ShaderResourceView*const idleSRV;
	ID3D11ShaderResourceView*const hoverSRV;
	ID3D11ShaderResourceView*const pressSRV;

	Geometrics::Plane bound;
};


class UICanvas
{
public:
	UICanvas(float width, float height);
	~UICanvas();

	// screen coordinate
	void Add(std::string id, XMFLOAT2 pivot, float width, float height, float zDepth, ID3D11ShaderResourceView* srv, UINT maxSliceIdx = 1, UINT slicePerSec = 1);
	void AddButton(std::string id, XMFLOAT2 pivot, XMFLOAT2 size, ID3D11ShaderResourceView* idleSRV, ID3D11ShaderResourceView* hoverSRV, ID3D11ShaderResourceView* pressSRV);

	void Remove(std::string id);

	void Update(float spf);
	void Render();

	const float totalWidth, totalHeight;

private:

	std::unordered_map<std::string, UI*> UIs;
	Camera* camera;
};

