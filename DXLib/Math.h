#pragma once

#ifndef _MATH
#define _MATH

#include <random>
#include <DirectXMath.h>

using namespace DirectX;

#define DIR_FORWARD DirectX::XMFLOAT3(0,0,1)
#define DIR_RIGHT DirectX::XMFLOAT3(1,0,0)
#define DIR_UP DirectX::XMFLOAT3(0,1,0)

inline XMFLOAT2 operator+(XMFLOAT2 a, XMFLOAT2 b)
{
	return DirectX::XMFLOAT2(a.x + b.x, a.y + b.y);
}
inline DirectX::XMFLOAT2 operator-(DirectX::XMFLOAT2 a, DirectX::XMFLOAT2 b) 
{
	return DirectX::XMFLOAT2(a.x - b.x, a.y - b.y);
}
inline DirectX::XMFLOAT2 operator+(DirectX::XMFLOAT2 v, float f) {
	return DirectX::XMFLOAT2(v.x + f, v.y + f);
}
inline DirectX::XMFLOAT2 operator-(DirectX::XMFLOAT2 v, float f) {
	return DirectX::XMFLOAT2(v.x - f, v.y - f);
}
inline DirectX::XMFLOAT2 operator*(DirectX::XMFLOAT2 v, float f) {
	return DirectX::XMFLOAT2(v.x * f, v.y * f);
}

inline DirectX::XMFLOAT2 operator/(DirectX::XMFLOAT2 v, float f) {
	return DirectX::XMFLOAT2(v.x / f, v.y / f);
}
inline DirectX::XMFLOAT3 operator+(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b) {
	DirectX::XMFLOAT3 sum;
	sum.x = a.x + b.x;
	sum.y = a.y + b.y;
	sum.z = a.z + b.z;
	return sum;
}
inline DirectX::XMFLOAT3 operator+=(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b) {

	DirectX::XMFLOAT3 sum;
	sum.x = a.x+ b.x;
	sum.y = a.y + b.y;
	sum.z = a.z + b.z;
	return sum;
}
inline DirectX::XMFLOAT3 operator-(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b) {
	DirectX::XMFLOAT3 sum;
	sum.x = a.x - b.x;
	sum.y = a.y - b.y;
	sum.z = a.z - b.z;
	return sum;
}
inline DirectX::XMFLOAT3 operator-(DirectX::XMFLOAT3 v) {
	DirectX::XMFLOAT3 sum;
	sum.x = -v.x;
	sum.y = -v.y;
	sum.z = -v.z;
	return sum;
}
inline DirectX::XMFLOAT3 operator+(DirectX::XMFLOAT3 v, float f) {
	DirectX::XMFLOAT3 sum;
	sum.x = v.x + f;
	sum.y = v.y + f;
	sum.z = v.z + f;
	return sum;
}

inline DirectX::XMFLOAT3 operator-(DirectX::XMFLOAT3 v, float f) {
	DirectX::XMFLOAT3 sum;
	sum.x = v.x - f;
	sum.y = v.y - f;
	sum.z = v.z - f;
	return sum;
}
inline DirectX::XMFLOAT3 operator*(DirectX::XMFLOAT3 v, float f) {
	DirectX::XMFLOAT3 sum;
	sum.x = v.x * f;
	sum.y = v.y * f;
	sum.z = v.z * f;
	return sum;
}
inline DirectX::XMFLOAT3 operator*(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b) {
	DirectX::XMFLOAT3 sum;
	sum.x = a.x * b.x;
	sum.y = a.y * b.y;
	sum.z = a.z * b.z;
	return sum;
}
inline DirectX::XMFLOAT3 operator/(DirectX::XMFLOAT3 v, float f) {
	DirectX::XMFLOAT3 sum;
	sum.x = v.x / f;
	sum.y = v.y / f;
	sum.z = v.z / f;
	return sum;
}
inline void operator/=(DirectX::XMFLOAT4& v, float f) {
	v.x /= f;
	v.y /= f;
	v.z /= f;
	v.w /= f;
}
inline bool operator== (DirectX::XMFLOAT2 a, DirectX::XMFLOAT2 b) {
	return (a.x == b.x && a.y == b.y);
}
inline bool operator!= (DirectX::XMFLOAT2 a, DirectX::XMFLOAT2 b) {
	return (a.x != b.x || a.y != b.y);
}
inline bool operator== (DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b) {
	return (a.x == b.x && a.y == b.y && a.z == b.z);
}
inline bool operator!= (DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b) {
	return (a.x != b.x || a.y != b.y || a.z != b.z);
}
inline bool operator== (DirectX::XMFLOAT4 a, DirectX::XMFLOAT4 b) {
	return (a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w);
}
inline bool operator!= (DirectX::XMFLOAT4 a, DirectX::XMFLOAT4 b) {
	return (a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w);
}
inline DirectX::XMFLOAT4 operator*(DirectX::XMFLOAT4 v, float f) {
	return DirectX::XMFLOAT4(v.x * f, v.y * f, v.z * f, v.w * f);
}
inline float Clamp(float a, float b, float v) {
	return fmaxf(a, fminf(v, b));
}
inline float Lerp(float v1, float v2, float t) {
	return v1 + (v2 - v1) * Clamp(0, 1, t);
}
inline float Rand01() {
	return (rand() / (float)RAND_MAX);
}
inline float Length(DirectX::XMFLOAT2 v) {
	return sqrtf(v.x * v.x + v.y * v.y);
}
inline DirectX::XMFLOAT2 Cross(DirectX::XMFLOAT2 a, DirectX::XMFLOAT2 b) {
	DirectX::XMVECTOR v = DirectX::XMVector2Cross(XMLoadFloat2(&a), XMLoadFloat2(&b));
	DirectX::XMFLOAT2 f2;
	XMStoreFloat2(&f2, v);
	return f2;
}
inline float Dot(DirectX::XMFLOAT2 a, DirectX::XMFLOAT2 b) {
	DirectX::XMVECTOR v = DirectX::XMVector2Dot(XMLoadFloat2(&a), XMLoadFloat2(&b));
	DirectX::XMFLOAT2 f2;
	XMStoreFloat2(&f2, v);
	return f2.x;
}
inline DirectX::XMFLOAT3 Multiply(DirectX::XMFLOAT3 v, const DirectX::XMMATRIX& m) {
	DirectX::XMVECTOR mv = DirectX::XMVector3Transform(XMLoadFloat3(&v), m);
	DirectX::XMFLOAT3 f3;
	XMStoreFloat3(&f3, mv);
	return f3;
}
inline DirectX::XMFLOAT3 MultiplyDir(DirectX::XMFLOAT3 v, const DirectX::XMMATRIX& m) {
	DirectX::XMVECTOR mv = DirectX::XMVector3TransformNormal(XMLoadFloat3(&v), m);
	DirectX::XMFLOAT3 f3;
	XMStoreFloat3(&f3, mv);
	return f3;
}
inline float Length(DirectX::XMFLOAT3 v) {
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}
inline DirectX::XMFLOAT3 Cross(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b) {
	DirectX::XMVECTOR v = DirectX::XMVector3Cross(XMLoadFloat3(&a), XMLoadFloat3(&b));
	DirectX::XMFLOAT3 f3;
	XMStoreFloat3(&f3, v);
	return f3;
}
inline DirectX::XMFLOAT3 Lerp(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2, float t) {
	return v1 + (v2 - v1) * Clamp(0, 1, t);
}
inline float Dot(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b) {
	DirectX::XMVECTOR v = DirectX::XMVector3Dot(XMLoadFloat3(&a), XMLoadFloat3(&b));
	DirectX::XMFLOAT3 f3;
	XMStoreFloat3(&f3, v);
	return f3.x;
}
inline DirectX::XMFLOAT3 Normalize(DirectX::XMFLOAT3 v) {
	DirectX::XMVECTOR nv = DirectX::XMVector3Normalize(XMLoadFloat3(&v));
	DirectX::XMFLOAT3 f3;
	XMStoreFloat3(&f3, nv);
	return f3;
}
inline DirectX::XMFLOAT3 Abs(DirectX::XMFLOAT3 v) {
	DirectX::XMFLOAT3 sum;
	sum.x = abs(v.x);
	sum.y = abs(v.y);
	sum.z = abs(v.z);
	return sum;
}
inline float Max(DirectX::XMFLOAT3 v) {
	return DirectX::XMMax(v.x, DirectX::XMMax(v.y, v.z));
}
// radian between dir1, dir2
inline float Rad(DirectX::XMFLOAT3 d1, DirectX::XMFLOAT3 d2) {
	return acosf(Dot(d1, d2));
}
inline DirectX::XMFLOAT3 RotateFromDir(DirectX::XMFLOAT3 p, DirectX::XMFLOAT3 dir, float rad) {
	DirectX::XMFLOAT3 c = dir * Dot(dir, p);
	DirectX::XMFLOAT3 right = p - c;
	DirectX::XMFLOAT3 up = Cross(dir, right);

	return c + right * cosf(rad) + up * sinf(rad);
}

inline DirectX::XMFLOAT4 Multiply(DirectX::XMFLOAT4 v, const DirectX::XMMATRIX& m) {
	DirectX::XMVECTOR mv = DirectX::XMVector4Transform(XMLoadFloat4(&v), m);
	DirectX::XMFLOAT4 f4;
	XMStoreFloat4(&f4, mv);
	return f4;
}

namespace Geom {
	struct Sphere {
	public:
		DirectX::XMFLOAT3 p;
		float rad;

		Sphere() :p(DirectX::XMFLOAT3(0.0, 0.0, 0.0)), rad(1) 
		{
		}
		Sphere(DirectX::XMFLOAT3 p, float rad) :p(p), rad(rad) {}
	};
	struct PlaneInf {
	public:
		DirectX::XMFLOAT3 p;
		DirectX::XMFLOAT3 n;
		PlaneInf() :p(DirectX::XMFLOAT3(0, 0, 0)), n(DirectX::XMFLOAT3(0, 0, 0)) {}
		PlaneInf(DirectX::XMFLOAT3 p, DirectX::XMFLOAT3 n) :p(p), n(n) {}
	};
	struct Plane {
	public:
		DirectX::XMFLOAT3 c;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT3 up;
		DirectX::XMFLOAT3 right;
		DirectX::XMFLOAT2 rad;
		Plane() :c(DirectX::XMFLOAT3(0, 0, 0)), normal(DirectX::XMFLOAT3(0, 0, 0)), up(DirectX::XMFLOAT3(0, 0, 0)), right(DirectX::XMFLOAT3(0, 0, 0)), rad(DirectX::XMFLOAT2(0, 0)) {}
		Plane(DirectX::XMFLOAT3 c, DirectX::XMFLOAT3 normal, DirectX::XMFLOAT3 up, DirectX::XMFLOAT2 rad) :c(c), normal(normal), up(up), right(Cross(up, normal)), rad(rad) {}
	};
	struct Bound {
	public:
		DirectX::XMFLOAT3 minPos;
		DirectX::XMFLOAT3 maxPos;
		Bound() : minPos(DirectX::XMFLOAT3(0, 0, 0)), maxPos(DirectX::XMFLOAT3(0, 0, 0)) {}
		Bound(DirectX::XMFLOAT3 minPos, DirectX::XMFLOAT3 maxPos) :minPos(minPos), maxPos(maxPos) {}
	};
	struct Ray {

	public:
		DirectX::XMFLOAT3 o;
		DirectX::XMFLOAT3 d;

		Ray() :o(DirectX::XMFLOAT3(0, 0, 0)), d(DirectX::XMFLOAT3(0, 0, 0)) {}
		Ray(const DirectX::XMFLOAT3& o, const DirectX::XMFLOAT3& d) : o(o), d(d) {}

		DirectX::XMFLOAT3 operator() (float t) const {

			return o + d * t;
		}
	};
	struct Ray2D {
	public:
		DirectX::XMFLOAT2 p;
		DirectX::XMFLOAT2 d;

		Ray2D() {
			p = DirectX::XMFLOAT2(0, 0);
			d = DirectX::XMFLOAT2(0, 1);
		}
		Ray2D(DirectX::XMFLOAT2 pt, DirectX::XMFLOAT2 dir)
		{
			p = pt;
			d = dir;
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
		DirectX::XMFLOAT3 toRayPt = ray.o - sph.p;
		DirectX::XMFLOAT3 tempRight = Normalize(Cross(ray.d, toRayPt));
		DirectX::XMFLOAT3 dir = Cross(tempRight, ray.d);

		return (Dot(toRayPt, dir) < sph.rad);
	}
	inline bool IntersectRayPlaneInf(const Ray ray, const PlaneInf plane, DirectX::XMFLOAT3* itsPt)
	{
		float dirDot = Dot(ray.d, plane.n);
		if (dirDot == 0)
			return false;

		float t = Dot(plane.n, plane.p - ray.o) / dirDot;
		*itsPt = ray.o + ray.d * t;

		return true;
	}
	// considered infinite ray & both plane side
	inline bool IntersectRayPlane(Ray ray, Plane plane, DirectX::XMFLOAT3* itsPt)
	{
		float dirDot = Dot(ray.d, plane.normal);
		if (dirDot == 0)
			return false;

		float t = Dot(plane.normal, plane.c - ray.o) / dirDot;
		DirectX::XMFLOAT3 pt = ray.o + ray.d * t;
		float xDist = abs(Dot(plane.right, pt - plane.c));
		float yDist = abs(Dot(plane.up, pt - plane.c));
		if (itsPt)
			*itsPt = pt;

		return (
			(Dot(ray.d, pt - ray.o) > 0) && // check if ray is ahead of plane
			(xDist < plane.rad.x) &&  // check if hitPt is in x range
			(yDist < plane.rad.y) // check if hitPt is in y range
			);
	}
	inline bool Intersect2DRayLine(Ray2D ray, DirectX::XMFLOAT2 linePt1, DirectX::XMFLOAT2 linePt2)
	{
		DirectX::XMFLOAT2 o = ray.p;
		DirectX::XMFLOAT2 d = ray.d;
		DirectX::XMFLOAT2 a = linePt1;
		DirectX::XMFLOAT2 b = linePt2;

		if (a == b || d == DirectX::XMFLOAT2(0, 0))
			return false;
		/*
		±âº»
		o+t1*d = a+t2*(b-a)

		t1
		oX(b-a) + t1 * dX(b-a) = aX(b-a)
		t1 = |(a-o)X(b-a)| / |dX(b-a)|

		t2
		same way

		*/
		float t1 = ((a.x - o.x) * (b.y - a.y) - (a.y - o.y) * (b.x - a.x)) / (d.x * (b.y - a.y) - d.y * (b.x - a.x));
		float t2 = ((o.x - a.x) * d.y - (o.y - a.y) * d.x) / ((b.x - a.x) * d.y - (b.y - a.y) * d.x);

		return (t1 > 0 && 0 < t2 && t2 <= 1);
	}
}

#endif // !MATH