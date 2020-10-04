#pragma once

#ifndef _MATH
#define _MATH


#include <DirectXMath.h>

using namespace DirectX;

#define FORWARD XMFLOAT3(0,0,1)
#define RIGHT XMFLOAT3(1,0,0)
#define UP XMFLOAT3(0,1,0)


namespace DX {

	DXLIB_DLL XMFLOAT2 operator+(XMFLOAT2 a, XMFLOAT2 b);
	DXLIB_DLL XMFLOAT2 operator-(XMFLOAT2 a, XMFLOAT2 b);
	DXLIB_DLL XMFLOAT2 operator+(XMFLOAT2 v, float f);
	DXLIB_DLL XMFLOAT2 operator-(XMFLOAT2 v, float f);
	DXLIB_DLL XMFLOAT2 operator*(XMFLOAT2 v, float f);
	DXLIB_DLL XMFLOAT2 operator/(XMFLOAT2 v, float f);
	DXLIB_DLL XMFLOAT3 operator+(XMFLOAT3 a, XMFLOAT3 b);
	DXLIB_DLL void operator+=(XMFLOAT3& a, XMFLOAT3 b);
	DXLIB_DLL XMFLOAT3 operator-(XMFLOAT3 a, XMFLOAT3 b);
	DXLIB_DLL XMFLOAT3 operator-(XMFLOAT3 v);
	DXLIB_DLL XMFLOAT3 operator+(XMFLOAT3 v, float f);
	DXLIB_DLL XMFLOAT3 operator-(XMFLOAT3 v, float f);
	DXLIB_DLL XMFLOAT3 operator*(XMFLOAT3 v, float f);
	DXLIB_DLL XMFLOAT3 operator*(XMFLOAT3 a, XMFLOAT3 b);
	DXLIB_DLL XMFLOAT3 operator/(XMFLOAT3 v, float f);
	DXLIB_DLL void operator/=(XMFLOAT4& v, float f);
	DXLIB_DLL bool operator== (XMFLOAT3 a, XMFLOAT3 b);
	DXLIB_DLL bool operator!= (XMFLOAT3 a, XMFLOAT3 b);
	DXLIB_DLL bool operator== (XMFLOAT4 a, XMFLOAT4 b);
	DXLIB_DLL bool operator!= (XMFLOAT4 a, XMFLOAT4 b);
	DXLIB_DLL XMFLOAT4 operator*(XMFLOAT4 v, float f);
	DXLIB_DLL float Clamp(float a, float b, float v);
	DXLIB_DLL float Lerp(float v1, float v2, float t);
	DXLIB_DLL float Rand01();
	DXLIB_DLL float Length(XMFLOAT2 v);
	DXLIB_DLL XMFLOAT2 Cross(XMFLOAT2 a, XMFLOAT2 b);
	DXLIB_DLL float Dot(XMFLOAT2 a, XMFLOAT2 b);
	DXLIB_DLL XMFLOAT3 Multiply(XMFLOAT3 v, const XMMATRIX& m);
	DXLIB_DLL XMFLOAT3 MultiplyDir(XMFLOAT3 v, const XMMATRIX& m);
	DXLIB_DLL float Length(XMFLOAT3 v);
	DXLIB_DLL XMFLOAT3 Cross(XMFLOAT3 a, XMFLOAT3 b);
	DXLIB_DLL XMFLOAT3 Lerp(XMFLOAT3 v1, XMFLOAT3 v2, float t);
	DXLIB_DLL float Dot(XMFLOAT3 a, XMFLOAT3 b);
	DXLIB_DLL XMFLOAT3 Normalize(XMFLOAT3 v);
	DXLIB_DLL XMFLOAT3 Abs(XMFLOAT3 v);
	DXLIB_DLL float Max(XMFLOAT3 v);
	// radian between dir1, dir2
	DXLIB_DLL float Rad(XMFLOAT3 d1, XMFLOAT3 d2);
	DXLIB_DLL XMFLOAT3 RotateFromDir(XMFLOAT3 p, XMFLOAT3 dir, float rad);

	DXLIB_DLL XMFLOAT4 Multiply(XMFLOAT4 v, const XMMATRIX& m);
}


#endif // !MATH