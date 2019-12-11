#pragma once

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
class RasterizerState;
class UICanvas;

struct SHADER_STD_TRANSF;
class UI
{
public:
	UI(UICanvas* canvas, XMFLOAT2 pivot, XMFLOAT2 size, float zDepth, ID3D11ShaderResourceView * srv);
	~UI();

	void Fade(float offset);
	bool Enabled() { return enabled; }
	void SetEnabled(bool e) { enabled = e; }
	float GetTransp() { return transp; }

protected:
	bool enabled;

	Quad* quad;
	Transform* transform;
	VShader* vs;
	PShader* ps;
	DepthStencilState* dsState;
	BlendState* blendState;
	RasterizerState* rsState;
	ID3D11ShaderResourceView* srv;

	XMFLOAT2 size;


	friend class UICanvas;
	virtual void Update(const Camera* camera);

	virtual void Render(const Camera* camera)const;
private:
	float transp;
};

class UIButton : public UI, public Subject
{
public:
	UIButton(UICanvas* canvas, UINT trigID, const void* trigData, XMFLOAT2 pivot, XMFLOAT2 size, ID3D11ShaderResourceView* idleSRV, ID3D11ShaderResourceView* hoverSRV, ID3D11ShaderResourceView* pressSRV);

private:
	friend class UICanvas;

	void Update(const Camera* camera) override;

	void Render(const Camera* camera)const override;


	ID3D11ShaderResourceView*const idleSRV;
	ID3D11ShaderResourceView*const hoverSRV;
	ID3D11ShaderResourceView*const pressSRV;

	Geometrics::Plane bound;

	UINT triggerID;
	const void* triggerData;
};


class UICanvas
{
public:
	UICanvas(float width, float height);
	~UICanvas();

	void Update(float spf);
	void Render(UINT sceneDepth);

	void Add(UI* ui);
	const float totalWidth, totalHeight;

private:
	std::unordered_set<UI*> UIs;
	Camera* camera;
};

