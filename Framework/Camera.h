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
	Camera(std::string key, FRAME_KIND frameKind, float screenWidth, float screenHeight, float nearPlane, float farPlane, float verticalViewRad, float aspectRatio);
	~Camera();
	void SetMain();
	void SetFrame(const FRAME_KIND fKind, XMFLOAT2 orthoSize, const float nearPlane, const float farPlane, const float verticalViewAngle, const float aspectRatio);
	void Capture(Scene* scene, ID3D11RenderTargetView** rtv, ID3D11DepthStencilView* dsv, D3D11_VIEWPORT vp);
	void Volume();

	XMMATRIX ProjMat(int zOrder) {
		return projMats[zOrder];
	}
	XMMATRIX ShadowMapVPMat() const
	{
		return viewMat * stdProjMat;
	}
	XMMATRIX VPMat(int zOrder)const
	{
		return viewMat * projMats[zOrder];
	}

	void SetPos(XMFLOAT3 pos);
	void SetRot(XMFLOAT3 forward);
	void SetRot(XMFLOAT3 forward, XMFLOAT3 up);

	const std::string key;

	FRAME_KIND GetFrame()const { return curFrame; }
	XMFLOAT2 GetSize()const { return size; }
	float GetN()const { return n; }
	float GetF()const { return f; }
	float GetVRad()const { return verticalRadian; }
	float GetAspectRatio()const { return aspectRatio; }
	XMFLOAT3 GetForward()const;
	XMFLOAT3 GetRight()const;
	XMFLOAT3 GetPos()const;

private:
	void SetView();
	Transform* transform;
	XMMATRIX stdProjMat;
	XMMATRIX* projMats;
	XMMATRIX viewMat;

	FRAME_KIND curFrame;
	XMFLOAT2 size;
	float n, f;
	float verticalRadian;
	float aspectRatio;
};

