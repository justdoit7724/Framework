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

class Camera
{
public:
	Camera(std::string key, const FRAME_KIND frameKind, UINT screenWidth, UINT screenHeight, const float nearPlane, const float farPlane, const float verticalViewAngle, const float aspectRatio, const XMFLOAT3 firstPos, const XMFLOAT3 _forward, const XMFLOAT3 _up);
	~Camera();
	void SetMain();
	void SetFrame(const FRAME_KIND fKind, UINT screenWidth, UINT screenHeight, const float nearPlane, const float farPlane, const float verticalViewAngle, const float aspectRatio);
	void Update(float spf);

	XMMATRIX ViewMat();
	XMMATRIX ProjMat(int zOrder) {
		return projMats[zOrder];
	}
	XMMATRIX VPMat(int zOrder)
	{
		return ViewMat() * projMats[zOrder];
	}

	Transform* transform;

	const std::string key;

private:
	XMMATRIX* projMats;

	FRAME_KIND curFrame;
};

