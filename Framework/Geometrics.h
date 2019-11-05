#pragma once
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <assert.h>
#include <limits>

using namespace DirectX;

#define FORWARD XMFLOAT3(0,0,1)
#define RIGHT XMFLOAT3(1,0,0)
#define UP XMFLOAT3(0,1,0)

inline float Lerp(float t, float v1, float v2) {
	return v1 + (v2 - v1)*t;
}
inline float Clamp(float a, float b, float v)
{
	return fmaxf(a, fminf(v, b));
}
inline float Rand01() {
	return (rand() / (float)RAND_MAX);
}

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
	return XMFLOAT2(v.x*f, v.y*f);
}
inline XMFLOAT2 operator/(XMFLOAT2 v, float f)
{
	return XMFLOAT2(v.x / f, v.y / f);
}
inline float Length(XMFLOAT2 v)
{
	return sqrt(v.x*v.x + v.y*v.y);
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
inline XMFLOAT3 operator+(XMFLOAT3 a, XMFLOAT3 b)
{
	return XMFLOAT3(a.x + b.x, a.y + b.y, a.z + b.z);
}
inline void operator+=(XMFLOAT3& a, XMFLOAT3 b)
{
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
}
inline XMFLOAT3 operator-(XMFLOAT3 a, XMFLOAT3 b)
{
	return XMFLOAT3(a.x - b.x, a.y - b.y, a.z - b.z);
}
inline XMFLOAT3 operator-(XMFLOAT3 v)
{
	return XMFLOAT3(-v.x, -v.y, -v.z);
}
inline XMFLOAT3 operator+(XMFLOAT3 v, float f)
{
	return XMFLOAT3(v.x + f, v.y + f, v.z + f);
}
inline XMFLOAT3 operator-(XMFLOAT3 v, float f)
{
	return XMFLOAT3(v.x - f, v.y - f, v.z - f);
}
inline XMFLOAT3 operator*(XMFLOAT3 v, float f)
{
	return XMFLOAT3(v.x*f, v.y*f, v.z*f);
}
inline XMFLOAT3 operator*(XMFLOAT3 a, XMFLOAT3 b)
{
	return XMFLOAT3(a.x * b.x, a.y * b.y, a.z * b.z);
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
inline XMFLOAT3 operator/(XMFLOAT3 v, float f)
{
	return XMFLOAT3(v.x / f, v.y / f, v.z / f);
}
inline float Length(XMFLOAT3 v)
{
	return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}
inline XMFLOAT3 Cross(XMFLOAT3 a, XMFLOAT3 b)
{
	XMVECTOR v = XMVector3Cross(XMLoadFloat3(&a), XMLoadFloat3(&b));
	XMFLOAT3 f3;
	XMStoreFloat3(&f3, v);
	return f3;
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
	return XMFLOAT3(abs(v.x), abs(v.y), abs(v.z));
}
inline float Max(XMFLOAT3 v)
{
	return XMMax(v.x, XMMax(v.y, v.z));
}
// radian between dir1, dir2
inline float Rad(XMFLOAT3 d1, XMFLOAT3 d2)
{
	return acos(Dot(d1, d2));
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
inline XMFLOAT4 Multiply(XMFLOAT4 v, const XMMATRIX& m)
{
	XMVECTOR mv = XMVector4Transform(XMLoadFloat4(&v), m);
	XMFLOAT4 f4;
	XMStoreFloat4(&f4, mv);
	return f4;
}

namespace Geometrics {
	class Sphere {
	public:
		XMFLOAT3 p;
		float rad;

		Sphere() {}
		Sphere(XMFLOAT3 p, float rad) :p(p), rad(rad) {}
	};
	class Ray {

	public:
		XMFLOAT3 o;
		XMFLOAT3 d;

		Ray() {}
		Ray(const XMFLOAT3& o, const XMFLOAT3& d) : o(o), d(d) {}

		XMFLOAT3 operator() (float t) const {

			return o + d * t;
		}
	};

	
}

inline bool IntersectInPlaneSphere(XMFLOAT3 pC, XMFLOAT3 pN, Geometrics::Sphere sph)
{
	float proj = Dot(sph.p - pC, pN);

	return ((proj + sph.rad) > 0);
}
inline bool IntersectRaySphere(Geometrics::Ray ray, Geometrics::Sphere sph)
{
	XMFLOAT3 toRayPt = ray.o - sph.p;
	XMFLOAT3 tempRight = Normalize(Cross(ray.d, toRayPt));
	XMFLOAT3 dir = Cross(tempRight, ray.d);

	return (Dot(toRayPt, dir) < sph.rad);
}

