#pragma once
#include <string>
#include <unordered_set>
#include <vector>
#include "DX_info.h"
#include "ObserverDP.h"
#include "Geometrics.h"
#include "Network.h"

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
class UICanvas;

struct SHADER_STD_TRANSF;
class UI
{
public:
	UI(UICanvas* canvas, XMFLOAT2 pivot, XMFLOAT2 size, float zDepth, ID3D11ShaderResourceView * srv);

	void Fade(float offset);
	bool Enabled() { return enabled; }
	void SetEnabled(bool e) { enabled = e; }
	float GetTransp() { return transp; }

protected:
	~UI();
	bool enabled;

	Quad* quad;
	Transform* transform;
	VShader* vs;
	PShader* ps;
	DepthStencilState* dsState;
	BlendState* blendState;
	ID3D11ShaderResourceView* srv;

	XMFLOAT2 size;


	friend class UICanvas;
	virtual void Update(const Camera* camera);

	virtual void Render(const Camera* camera)const;
private:
	float transp;
};

class UIButton : public UI, public Subject, public IDebug
{
public:
	UIButton(UICanvas* canvas, const void* data, UINT dataSize, XMFLOAT2 pivot, XMFLOAT2 size, ID3D11ShaderResourceView* idleSRV, ID3D11ShaderResourceView* hoverSRV, ID3D11ShaderResourceView* pressSRV);
	void Visualize()override;
private:
	~UIButton();
	friend class UICanvas;

	void Update(const Camera* camera) override;

	void Render(const Camera* camera)const override;


	ID3D11ShaderResourceView*const idleSRV;
	ID3D11ShaderResourceView*const hoverSRV;
	ID3D11ShaderResourceView*const pressSRV;

	Geometrics::Plane bound;

	void* triggerData;
};


class UICanvas : public IDebug
{
public:
	UICanvas(float width, float height);
	~UICanvas();

	void Update(float spf);
	void Render(UINT sceneDepth);

	void Add(UI* ui);
	const float totalWidth, totalHeight;

	void Visualize() override;

private:
	std::unordered_set<UI*> UIs;
	Camera* camera;
};

