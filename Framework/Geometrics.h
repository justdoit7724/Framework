#pragma once
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <assert.h>
#include <limits>

#ifndef _GEOMETRICS
#define _GEOMETRICS

using namespace DirectX;

#define FORWARD XMFLOAT3(0,0,1)
#define RIGHT XMFLOAT3(1,0,0)
#define UP XMFLOAT3(0,1,0)

inline float Clamp(float a, float b, float v)
{
	return fmaxf(a, fminf(v, b));
}
inline float Lerp(float v1, float v2, float t) {
	return v1 + (v2 - v1)*Clamp(0,1,t);
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
	return sqrtf(v.x * v.x + v.y*v.y);
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
	return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
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
	return XMFLOAT3(abs(v.x), abs(v.y), abs(v.z));
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
inline XMFLOAT3 RotateFromDir(XMFLOAT3 p, XMFLOAT3 dir, float rad)
{
	XMFLOAT3 c = dir * Dot(dir, p);
	XMFLOAT3 right = p - c;
	XMFLOAT3 up = Cross(dir, right);

	return c + right * cosf(rad) + up * sinf(rad);
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

		Sphere():p(XMFLOAT3(0,0,0)), rad(1) {}
		Sphere(XMFLOAT3 p, float rad) :p(p), rad(rad) {}
	};
	class PlaneInf {
	public:
		XMFLOAT3 p;
		XMFLOAT3 n;
		PlaneInf():p(XMFLOAT3(0, 0, 0)), n(XMFLOAT3(0, 0, 0)) {}
		PlaneInf(XMFLOAT3 p, XMFLOAT3 n) :p(p), n(n) {}
	};
	class Plane {
	public:
		XMFLOAT3 c;
		XMFLOAT3 normal;
		XMFLOAT3 up;
		XMFLOAT3 right;
		XMFLOAT2 rad;
		Plane() :c(XMFLOAT3(0,0,0)), normal(XMFLOAT3(0, 0, 0)), up(XMFLOAT3(0, 0, 0)), right(XMFLOAT3(0, 0, 0)), rad(XMFLOAT2(0, 0)) {}
		Plane(XMFLOAT3 c, XMFLOAT3 normal, XMFLOAT3 up, XMFLOAT2 rad) :c(c), normal(normal),up(up), right(Cross(up,normal)), rad(rad) {}
	};
	class Bound {
	public:
		XMFLOAT3 center;
		XMFLOAT3 rad;
		Bound(): center(XMFLOAT3(0, 0, 0)), rad(XMFLOAT3(0, 0, 0)){}
		Bound(XMFLOAT3 c, XMFLOAT3 rad) :center(c), rad(rad) {}
	};
	class Ray {

	public:
		XMFLOAT3 o;
		XMFLOAT3 d;

		Ray():o(XMFLOAT3(0,0,0)), d(XMFLOAT3(0,0,0)) {}
		Ray(const XMFLOAT3& o, const XMFLOAT3& d) : o(o), d(d) {}

		XMFLOAT3 operator() (float t) const {

			return o + d * t;
		}
	};

	inline bool IntersectInPlaneSphere(PlaneInf plane, Sphere sph)
	{
		float proj = Dot(sph.p - plane.p, plane.n);

		return ((proj + sph.rad) > 0);
	}
	// infinite ray
	inline bool IntersectRaySphere(Ray ray, Sphere sph)
	{
		XMFLOAT3 toRayPt = ray.o - sph.p;
		XMFLOAT3 tempRight = Normalize(Cross(ray.d, toRayPt));
		XMFLOAT3 dir = Cross(tempRight, ray.d);

		return (Dot(toRayPt, dir) < sph.rad);
	}
	inline bool IntersectRayPlaneInf(const Ray ray, const PlaneInf plane, XMFLOAT3* itsPt)
	{
		float dirDot = Dot(ray.d, plane.n);
		if (dirDot == 0)
			return false;

		float t = Dot(plane.n, plane.p - ray.o) / dirDot;
		*itsPt = ray.o + ray.d * t;

		return true;
	}
	// considered infinite ray & both plane side
	inline bool IntersectRayPlane(Ray ray, Plane plane, XMFLOAT3* itsPt)
	{
		float dirDot = Dot(ray.d, plane.normal);
		if (dirDot == 0)
			return false;

		float t = Dot(plane.normal, plane.c - ray.o) / dirDot;
		XMFLOAT3 pt = ray.o + ray.d * t;
		float xDist = abs(Dot(plane.right, pt - plane.c));
		float yDist = abs(Dot(plane.up, pt - plane.c));
		if (itsPt)
			* itsPt = pt;

		return (xDist < plane.rad.x) && (yDist < plane.rad.y);
	}
}

#endif

