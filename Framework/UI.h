#pragma once

#include "ObserverDP.h"
#include "Object.h"

class Transform;
class Camera;
class UICanvas;

struct SHADER_STD_TRANSF;
class UI : public Object
{
public:
	UI(XMFLOAT2 pivot, XMFLOAT2 size, float zDepth, ID3D11ShaderResourceView * srv);
	~UI();

	virtual void Update(UICanvas* canvas);

protected:

	ID3D11ShaderResourceView* srv;

	XMFLOAT2 size;
};

class UIButton : public UI, public Subject
{
public:
	typedef void BtnFunc(const void*);
	
	UIButton(BtnFunc* trigFunc, const void* trigData, XMFLOAT2 pivot, XMFLOAT2 size, ID3D11ShaderResourceView* idleSRV, ID3D11ShaderResourceView* hoverSRV, ID3D11ShaderResourceView* pressSRV);

	void Update(UICanvas* canvas) override;
private:


	ID3D11ShaderResourceView*const idleSRV;
	ID3D11ShaderResourceView*const hoverSRV;
	ID3D11ShaderResourceView*const pressSRV;

	Plane bound;

	BtnFunc* trigFunc;
	const void* triggerData;
};


class UICanvas
{
public:
	UICanvas();
	~UICanvas();

	void Update();

	const float totalWidth, totalHeight;

	const Camera* GetCamera() { return camera; }

private:
	Camera* camera;
};

