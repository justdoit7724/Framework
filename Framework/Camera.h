#pragma once
#include "DX_info.h"
#include "Geometrics.h"


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
class Buffer;

struct Frustum
{
	bool skip;
	Geometrics::PlaneInf front;
	Geometrics::PlaneInf back;
	Geometrics::PlaneInf right;
	Geometrics::PlaneInf left;
	Geometrics::PlaneInf top;
	Geometrics::PlaneInf bottom;

	Frustum() {}
};

class Camera
{
public:
	Camera(std::string key, FRAME_KIND frameKind, float screenWidth, float screenHeight, float nearPlane, float farPlane, float verticalViewRad, float aspectRatio, bool skipFrustum);
	Camera(FRAME_KIND frameKind, float screenWidth, float screenHeight, float nearPlane, float farPlane, float verticalViewRad, float aspectRatio, bool skipFrustum);
	~Camera();
	void SetFrame(const FRAME_KIND fKind, XMFLOAT2 orthoSize, const float nearPlane, const float farPlane, const float verticalViewAngle, const float aspectRatio);
	void Update();
	void SetView();

	XMMATRIX VMat()const { return viewMat; }
	XMMATRIX ProjMat(int zOrder)const {return projMats[zOrder];}
	XMMATRIX StdProjMat()const { return stdProjMat; }
	void Pick(OUT Geometrics::Ray* ray)const;

	const std::string key;

	FRAME_KIND GetFrame()const { return curFrame; }
	XMFLOAT2 GetSize()const { return size; }
	float GetN()const { return n; }
	float GetF()const { return f; }
	float GetVRad()const { return verticalRadian; }
	float GetAspectRatio()const { return aspectRatio; }

	const Frustum& GetFrustum()const { return frustum; }

	Transform* transform;

private:
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

