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
	void SetSRV(ID3D11ShaderResourceView* srv);
	void SetMulColor(XMFLOAT3 c);

protected:

	ID3D11ShaderResourceView* srv;

	XMFLOAT4 mulColor;
	XMFLOAT2 size;
};

class UIButton : public UI, public Subject
{
public:
	UIButton(XMFLOAT2 pivot, XMFLOAT2 size, ID3D11ShaderResourceView* idleSRV);

	void Update(UICanvas* canvas) override;

	void SetNotify(int id, const void* data) { notifyID = id; notifyData = data; }

private:

	int notifyID=NOTIFY_NONE;
	const void* notifyData=nullptr;

	Geometrics::Plane bound;
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

