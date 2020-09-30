// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H


#ifdef DXLIB_EXPORTS
#define DXLIB_DLL __declspec(dllexport)
#else
#define DXLIB_DLL __declspec(dllimport)
#endif

// add headers that you want to pre-compile here
#include "framework.h"

#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib,"d3d11.lib")


#ifndef LOAD_LIBRARY
#define LOAD_LIBRARY

//path : C:\Program Files (x86)\Windows Kits\10\Include\10.0.18362.0\um

#ifdef _WIN64

#pragma comment(lib, __FILE__"/../_Packages/Assimp.3.0.0/lib_x64/assimp.lib")

#ifdef _DEBUG
#pragma comment(lib, __FILE__"\\..\\_Packages\\directxtk_desktop_2015.2019.12.17.1\\lib_x64\\_Debug\\DirectXTK.lib")
#else
#pragma comment(lib, __FILE__"\\..\\_Packages\\directxtk_desktop_2015.2019.12.17.1\\lib_x64\\_Release\\DirectXTK.lib")
#endif

#else

#pragma comment(lib, __FILE__"/../_Packages/Assimp.3.0.0/lib_win32/assimp.lib")

#ifdef _DEBUG
#pragma comment(lib, __FILE__"\\..\\_Packages\\directxtk_desktop_2015.2019.12.17.1\\lib_win32\\_Debug\\DirectXTK.lib")
#else
#pragma comment(lib, __FILE__"\\..\\_Packages\\directxtk_desktop_2015.2019.12.17.1\\lib_win32\\_Release\\DirectXTK.lib")
#endif

#endif


#endif

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <assert.h>
#include <string>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <limits>
#include <memory>
#include <queue>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <fstream>

using namespace DirectX;

namespace DX {
	extern ID3D11Device* DX_Device;
	extern ID3D11DeviceContext* DX_DContext;
	extern float WND_WIDTH;
	extern float WND_HEIGHT;

#define FORWARD XMFLOAT3(0,0,1)
#define RIGHT XMFLOAT3(1,0,0)
#define UP XMFLOAT3(0,1,0)


#define MB(s) \
MessageBox(0,s,0,0);\
exit(-1);

#define r_assert(r)	\
assert(SUCCEEDED(r))

	inline XMFLOAT2 operator+(XMFLOAT2 a, XMFLOAT2 b)
	{
		return XMFLOAT2(a.x + b.x, a.y + b.y);
	}
	inline XMFLOAT2 operator-(XMFLOAT2 a, XMFLOAT2 b)
	{
		return XMFLOAT2(a.x - b.x, a.y - b.y);
	}
	inline XMFLOAT2 operator+(XMFLOAT2 v, float f)
	{
		return XMFLOAT2(v.x + f, v.y + f);
	}
	inline XMFLOAT2 operator-(XMFLOAT2 v, float f)
	{
		return XMFLOAT2(v.x - f, v.y - f);
	}
	inline XMFLOAT2 operator*(XMFLOAT2 v, float f)
	{
		return XMFLOAT2(v.x * f, v.y * f);
	}
	inline XMFLOAT2 operator/(XMFLOAT2 v, float f)
	{
		return XMFLOAT2(v.x / f, v.y / f);
	}
	inline XMFLOAT3 operator+(XMFLOAT3 a, XMFLOAT3 b)
	{
		XMFLOAT3 sum;
		sum.x = a.x + b.x;
		sum.y = a.y + b.y;
		sum.z = a.z + b.z;
		return sum;
	}
	inline void operator+=(XMFLOAT3& a, XMFLOAT3 b)
	{
		a.x += b.x;
		a.y += b.y;
		a.z += b.z;
	}
	inline XMFLOAT3 operator-(XMFLOAT3 a, XMFLOAT3 b)
	{
		XMFLOAT3 sum;
		sum.x = a.x - b.x;
		sum.y = a.y - b.y;
		sum.z = a.z - b.z;
		return sum;
	}
	inline XMFLOAT3 operator-(XMFLOAT3 v)
	{
		XMFLOAT3 sum;
		sum.x = -v.x;
		sum.y = -v.y;
		sum.z = -v.z;
		return sum;
	}
	inline XMFLOAT3 operator+(XMFLOAT3 v, float f)
	{
		XMFLOAT3 sum;
		sum.x = v.x + f;
		sum.y = v.y + f;
		sum.z = v.z + f;
		return sum;
	}
	inline XMFLOAT3 operator-(XMFLOAT3 v, float f)
	{
		XMFLOAT3 sum;
		sum.x = v.x - f;
		sum.y = v.y - f;
		sum.z = v.z - f;
		return sum;
	}
	inline XMFLOAT3 operator*(XMFLOAT3 v, float f)
	{

		XMFLOAT3 sum;
		sum.x = v.x * f;
		sum.y = v.y * f;
		sum.z = v.z * f;
		return sum;
	}
	inline XMFLOAT3 operator*(XMFLOAT3 a, XMFLOAT3 b)
	{
		XMFLOAT3 sum;
		sum.x = a.x * b.x;
		sum.y = a.y * b.y;
		sum.z = a.z * b.z;
		return sum;
	}
	inline XMFLOAT3 operator/(XMFLOAT3 v, float f)
	{
		XMFLOAT3 sum;
		sum.x = v.x / f;
		sum.y = v.y / f;
		sum.z = v.z / f;
		return sum;
	}
	inline void operator/=(XMFLOAT4& v, float f)
	{
		v.x /= f;
		v.y /= f;
		v.z /= f;
		v.w /= f;
	}
	inline bool operator== (XMFLOAT3 a, XMFLOAT3 b)
	{
		return (a.x == b.x && a.y == b.y && a.z == b.z);
	}
	inline bool operator!= (XMFLOAT3 a, XMFLOAT3 b)
	{
		return (a.x != b.x || a.y != b.y || a.z != b.z);
	}
	inline bool operator== (XMFLOAT4 a, XMFLOAT4 b)
	{
		return (a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w);
	}
	inline bool operator!= (XMFLOAT4 a, XMFLOAT4 b)
	{
		return (a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w);
	}
	inline XMFLOAT4 operator*(XMFLOAT4 v, float f) {

		return XMFLOAT4(v.x * f, v.y * f, v.z * f, v.w * f);
	}
	inline float Clamp(float a, float b, float v)
	{
		return fmaxf(a, fminf(v, b));
	}
	inline float Lerp(float v1, float v2, float t) {
		return v1 + (v2 - v1) * Clamp(0, 1, t);
	}
	inline float Rand01() {
		return (rand() / (float)RAND_MAX);
	}


	inline float Length(XMFLOAT2 v)
	{
		return sqrtf(v.x * v.x + v.y * v.y);
	}
	inline XMFLOAT2 Cross(XMFLOAT2 a, XMFLOAT2 b)
	{
		XMVECTOR v = XMVector2Cross(XMLoadFloat2(&a), XMLoadFloat2(&b));
		XMFLOAT2 f2;
		XMStoreFloat2(&f2, v);
		return f2;
	}
	inline float Dot(XMFLOAT2 a, XMFLOAT2 b)
	{
		XMVECTOR v = XMVector2Dot(XMLoadFloat2(&a), XMLoadFloat2(&b));
		XMFLOAT2 f2;
		XMStoreFloat2(&f2, v);
		return f2.x;
	}
	inline XMFLOAT3 Multiply(XMFLOAT3 v, const XMMATRIX& m)
	{
		XMVECTOR mv = XMVector3Transform(XMLoadFloat3(&v), m);
		XMFLOAT3 f3;
		XMStoreFloat3(&f3, mv);
		return f3;
	}
	inline XMFLOAT3 MultiplyDir(XMFLOAT3 v, const XMMATRIX& m)
	{
		XMVECTOR mv = XMVector3TransformNormal(XMLoadFloat3(&v), m);
		XMFLOAT3 f3;
		XMStoreFloat3(&f3, mv);
		return f3;
	}
	inline float Length(XMFLOAT3 v)
	{
		return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	}
	inline XMFLOAT3 Cross(XMFLOAT3 a, XMFLOAT3 b)
	{
		XMVECTOR v = XMVector3Cross(XMLoadFloat3(&a), XMLoadFloat3(&b));
		XMFLOAT3 f3;
		XMStoreFloat3(&f3, v);
		return f3;
	}
	inline XMFLOAT3 Lerp(XMFLOAT3 v1, XMFLOAT3 v2, float t)
	{
		return v1 + (v2 - v1) * Clamp(0, 1, t);
	}
	inline float Dot(XMFLOAT3 a, XMFLOAT3 b)
	{
		XMVECTOR v = XMVector3Dot(XMLoadFloat3(&a), XMLoadFloat3(&b));
		XMFLOAT3 f3;
		XMStoreFloat3(&f3, v);
		return f3.x;
	}
	inline XMFLOAT3 Normalize(XMFLOAT3 v)
	{
		XMVECTOR nv = XMVector3Normalize(XMLoadFloat3(&v));
		XMFLOAT3 f3;
		XMStoreFloat3(&f3, nv);
		return f3;
	}
	inline XMFLOAT3 Abs(XMFLOAT3 v)
	{
		XMFLOAT3 sum;
		sum.x = abs(v.x);
		sum.y = abs(v.y);
		sum.z = abs(v.z);
		return sum;
	}
	inline float Max(XMFLOAT3 v)
	{
		return XMMax(v.x, XMMax(v.y, v.z));
	}
	// radian between dir1, dir2
	inline float Rad(XMFLOAT3 d1, XMFLOAT3 d2)
	{
		return acosf(Dot(d1, d2));
	}
	inline XMFLOAT3 RotateFromDir(XMFLOAT3 p, XMFLOAT3 dir, float rad)
	{
		XMFLOAT3 c = dir * Dot(dir, p);
		XMFLOAT3 right = p - c;
		XMFLOAT3 up = Cross(dir, right);

		return c + right * cosf(rad) + up * sinf(rad);
	}

	inline XMFLOAT4 Multiply(XMFLOAT4 v, const XMMATRIX& m)
	{
		XMVECTOR mv = XMVector4Transform(XMLoadFloat4(&v), m);
		XMFLOAT4 f4;
		XMStoreFloat4(&f4, mv);
		return f4;
	}

}


#endif //PCH_H
