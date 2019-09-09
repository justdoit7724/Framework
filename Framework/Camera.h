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

class Camera
{
public:
	Camera(const FRAME_KIND	frameKind, const float screenWidth, const float screenHeight, const float nearPlane, const float farPlane, const float verticalViewAngle, const float aspectRatio, const XMFLOAT3 firstPos, const XMFLOAT3 _forward, const XMFLOAT3 _up);
	void SetFrame(const FRAME_KIND fKind, const float screenWidth, const float screenHeight, const float nearPlane, const float farPlane, const float verticalViewAngle, const float aspectRatio);
	void SetTransform(const XMFLOAT3 firstPos, const XMFLOAT3 _forward, const XMFLOAT3 _up);
	void Update(float spf);

	XMFLOAT3 Pos(){return pos;}
	XMFLOAT3 Forward() {return forward;}
	XMMATRIX ViewMat() {return viewMat;}
	XMMATRIX ProjMat(int zOrder) {return projMats[zOrder];}
	XMMATRIX VPMat(int zOrder) {return viewMat*projMats[zOrder]; }

private:
	XMFLOAT3 pos;
	XMFLOAT3 forward;
	XMFLOAT3 up;
	XMFLOAT3 right;
	XMMATRIX viewMat;
	XMMATRIX* projMats;

	FRAME_KIND curFrame;
};

