#pragma once
#include "DX_info.h"
#include "Geometrics.h"


enum FRAME_KIND {
	FRAME_KIND_PERSPECTIVE,
	FRAME_KIND_ORTHOGONAL
};

class Transform;
class Scene;
class Buffer;

struct Frustum
{
	bool skip;
	Math::PlaneInf front;
	Math::PlaneInf back;
	Math::PlaneInf right;
	Math::PlaneInf left;
	Math::PlaneInf top;
	Math::PlaneInf bottom;

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

	const XMMATRIX& VMat()const { return viewMat; }
	const XMMATRIX& ProjMat()const {return projMat;}
	void Pick(OUT Math::Ray* ray)const;

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
	XMMATRIX projMat;
	XMMATRIX viewMat;

	FRAME_KIND curFrame;
	XMFLOAT2 size;
	float n, f;
	float verticalRadian;
	float aspectRatio;

	Frustum frustum;
};

