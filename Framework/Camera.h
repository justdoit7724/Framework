#pragma once
#include "DX_info.h"

enum FRAME_KIND {
	FRAME_KIND_PERSPECTIVE,
	FRAME_KIND_ORTHOGONAL
};

#define Z_ORDER_UI			0
#define Z_ORDER_HIGHER		1
#define Z_ORDER_STANDARD	2
#define Z_ORDER_LOWER		3
#define Z_ORDER_BACKGROUND	4

class Transform;
class Scene;

class Camera
{
public:
	Camera(std::string key, const Camera* camera);
	Camera(std::string key, const FRAME_KIND frameKind, float screenWidth, float screenHeight, const float nearPlane, const float farPlane, const float verticalViewAngle, const float aspectRatio, const XMFLOAT3 firstPos, const XMFLOAT3 _forward, const XMFLOAT3 _up);
	~Camera();
	void SetMain();
	void SetFrame(const FRAME_KIND fKind, XMFLOAT2 orthoSize, const float nearPlane, const float farPlane, const float verticalViewAngle, const float aspectRatio);
	void Capture(Scene* scene, ID3D11RenderTargetView** rtv, ID3D11DepthStencilView* dsv, D3D11_VIEWPORT vp);
	void Volume();

	XMMATRIX ViewMat()const;
	XMMATRIX ProjMat(int zOrder) {
		return projMats[zOrder];
	}
	XMMATRIX VPMat(int zOrder)const
	{
		return ViewMat() * projMats[zOrder];
	}

	Transform* transform;

	const std::string key;

	FRAME_KIND GetFrame()const { return curFrame; }
	XMFLOAT2 GetSize()const { return size; }
	float GetN()const { return n; }
	float GetF()const { return f; }
	float GetVRad()const { return verticalRadian; }
	float GetAspectRatio()const { return aspectRatio; }

private:
	XMMATRIX* projMats;

	FRAME_KIND curFrame;
	XMFLOAT2 size;
	float n, f;
	float verticalRadian;
	float aspectRatio;
};

