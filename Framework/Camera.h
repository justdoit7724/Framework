#pragma once
#include "DX_info.h"
#include "Network.h"

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

struct Frustum
{
	XMFLOAT3 sidePt, nPt, fPt;
	XMFLOAT3 nN, fN;
	XMFLOAT3 rN, lN;
	XMFLOAT3 tN, bN;
};

class Camera : public IDebug
{
public:
	Camera(std::string key, const Camera* camera);
	Camera(const Camera* camera);
	Camera(std::string key, FRAME_KIND frameKind, float screenWidth, float screenHeight, float nearPlane, float farPlane, float verticalViewRad, float aspectRatio);
	Camera(FRAME_KIND frameKind, float screenWidth, float screenHeight, float nearPlane, float farPlane, float verticalViewRad, float aspectRatio);
	~Camera();
	void SetMain();
	void SetFrame(const FRAME_KIND fKind, XMFLOAT2 orthoSize, const float nearPlane, const float farPlane, const float verticalViewAngle, const float aspectRatio);
	void Update();
	void Visualize() override;

	XMMATRIX VMat()const { return viewMat; }
	XMMATRIX ProjMat(int zOrder)const {return projMats[zOrder];}
	XMMATRIX ShadowPMat()const { return stdProjMat; }

	const std::string key;

	FRAME_KIND GetFrame()const { return curFrame; }
	XMFLOAT2 GetSize()const { return size; }
	float GetN()const { return n; }
	float GetF()const { return f; }
	float GetVRad()const { return verticalRadian; }
	float GetAspectRatio()const { return aspectRatio; }

	const Frustum* GetFrustum()const { return &frustum; }

	Transform* transform;

private:
	void SetView();
	XMMATRIX stdProjMat;
	XMMATRIX* projMats;
	XMMATRIX viewMat;

	FRAME_KIND curFrame;
	XMFLOAT2 size;
	float n, f;
	float verticalRadian;
	float aspectRatio;

	Frustum frustum;
};

