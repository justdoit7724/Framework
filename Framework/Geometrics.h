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
inline XMFLOAT3 operator*(XMFLOAT3 v, const XMMATRIX& m)
{
	XMVECTOR mv = XMVector3Transform(XMLoadFloat3(&v), m);
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
inline XMFLOAT4 operator/=(XMFLOAT4& v, float f)
{
	return XMFLOAT4(v.x / f, v.y / f, v.z / f, v.w/f);
}
inline bool operator== (XMFLOAT4 a, XMFLOAT4 b)
{
	return (a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w);
}
inline bool operator!= (XMFLOAT4 a, XMFLOAT4 b)
{
	return (a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w);
}
inline XMFLOAT4 operator*(XMFLOAT4 v, const XMMATRIX& m)
{
	XMVECTOR mv = XMVector4Transform(XMLoadFloat4(&v), m);
	XMFLOAT4 f4;
	XMStoreFloat4(&f4, mv);
	return f4;
}

namespace Geometrics
{
	class Medium;
	class Ray {

	public:
		XMFLOAT3 o;
		XMFLOAT3 d;

		mutable float tMax;
		float time;

		const Medium *medium;

		Ray() : tMax(FLT_MAX), time(0.f), medium(nullptr) {}
		Ray(const XMFLOAT3& o, const XMFLOAT3& d, float tMax = FLT_MAX, float time = 0.f, const Medium *medium = nullptr) : o(o), d(d), tMax(tMax), time(time), medium(medium) {}
		
		XMFLOAT3 operator() (float t) const {

			return o + d * t;
		}
	};

	class RayDifferential : public Ray
	{
	public:
		bool hasDifferentials;
		XMFLOAT3 rxOrigin, ryOrigin;
		XMFLOAT3 rxDir, ryDir;

		RayDifferential() {
			hasDifferentials = false;
		}
		RayDifferential(const XMFLOAT3& o, const XMFLOAT3& d, float tMax = FLT_MAX, float time = 0.f, const Medium* medium = nullptr)
			: Ray(XMFLOAT3(0,0,0), d, tMax, time, medium) {
			hasDifferentials = false;
		}
		RayDifferential(const Ray& ray) : Ray(ray) {
			hasDifferentials = false;
		}

		void ScaleDifferentials(float s)
		{
			rxOrigin = o + (rxOrigin - o)*s;
			ryOrigin = o + (ryOrigin - o)*s;
			rxDir = d + (rxDir - d)*s;
			ryDir = d + (ryDir - d)*s;
		}
	};

	class Bounds2f
	{
	public:
		Bounds2f() {
			
			float minNum = (std::numeric_limits<float>::lowest)();
			float maxNum = (std::numeric_limits<float>::max)();
			pMin = XMFLOAT2(maxNum, maxNum);
			pMax = XMFLOAT2(minNum, minNum);
		}
		Bounds2f(const XMFLOAT2& p) : pMin(p), pMax(p) {}
		Bounds2f(const XMFLOAT2& p1, const XMFLOAT2& p2)
		{
			pMin = XMFLOAT2(fminf(p1.x, p2.x), fminf(p1.y, p2.y));
			pMax = XMFLOAT2(fmaxf(p1.x, p2.x), fmaxf(p1.y, p2.y));
		}

		XMFLOAT2 Diagonal() const {
			return pMax - pMin;
		}
		float Area() const {
			XMFLOAT2 d = Diagonal();
			return (d.x*d.y);
		}



		XMFLOAT2 pMin, pMax;
	};

	class Bounds3f
	{
	public:
		Bounds3f() {
			float minNum = (std::numeric_limits<float>::lowest)();
			float maxNum = (std::numeric_limits<float>::max)();
			pMin = XMFLOAT3(maxNum, maxNum, maxNum);
			pMax = XMFLOAT3(minNum, minNum, minNum);
		}
		Bounds3f(const XMFLOAT3& p) : pMin(p), pMax(p) {}
		Bounds3f(const XMFLOAT3&p1, const XMFLOAT3 &p2)
			: 
			pMin(fminf(p1.x, p2.x), fminf(p1.y, p2.y), fminf(p1.z, p2.z)),
			pMax(fmaxf(p1.x, p2.x), fmaxf(p1.y, p2.y), fmaxf(p1.z, p2.z)) {}

		const XMFLOAT3& operator[] (int i) const {

			if (i == 0) return pMin;
			else return pMax;
		}
		XMFLOAT3 Corner(int corner) const {
			return XMFLOAT3(
				(corner & 1)? pMax.x : pMin.x, 
				(corner & 2)? pMax.y : pMin.y,
				(corner & 4)? pMax.z : pMin.z);
		}
		
		XMFLOAT3 Diagonal() const {
			return pMax - pMin;
		}
		float SurfaceArea() const {
			XMFLOAT3 d = Diagonal();
			return 2 * (d.x*d.y + d.x*d.z + d.y*d.z);
		}
		float Volume() const {
			XMFLOAT3 d = Diagonal();
			return d.x*d.y*d.z;
		}
		int MaxExtentIdx() const {
			XMFLOAT3 d = Diagonal();
			if (d.x > d.y && d.x > d.z)
				return 0;
			else if (d.y > d.z)
				return 1;
			else
				return 2;
		}

		XMFLOAT3 Lerp(const XMFLOAT3& t) const {
			return XMFLOAT3(
				::Lerp(t.x, pMin.x, pMax.x),
				::Lerp(t.y, pMin.y, pMax.y),
				::Lerp(t.z, pMin.z, pMax.z));
		}

		XMFLOAT3 Offset(const XMFLOAT3& p) const {
			XMFLOAT3 o = p - pMin;
			if (pMax.x > pMin.x) o.x /= (pMax.x - pMin.x);
			if (pMax.y > pMin.y) o.y /= (pMax.y - pMin.y);
			if (pMax.z > pMin.z) o.z /= (pMax.z - pMin.z);
		}

		void BoundingSphere(XMFLOAT3 *center, float* radius) const
		{
			*center = (pMin + pMax) / 2;
			*radius = Length(*center - pMax);
		}

		XMFLOAT3 pMin, pMax;
	};

	inline Bounds3f Union(const Bounds3f &b, const XMFLOAT3 &p) {
		return Bounds3f(
			XMFLOAT3(
				fminf(b.pMin.x, p.x),
				fminf(b.pMin.y, p.y),
				fminf(b.pMin.z, p.z)),
			XMFLOAT3(
				fmaxf(b.pMax.x, p.x),
				fmaxf(b.pMax.y, p.y),
				fmaxf(b.pMax.z, p.z)));
	}
	inline Bounds3f Union(const Bounds3f &b1, const Bounds3f &b2) {
		return Bounds3f(
			XMFLOAT3(
				fminf(b1.pMin.x, b2.pMin.x),
				fminf(b1.pMin.y, b2.pMin.y),
				fminf(b1.pMin.z, b2.pMin.z)),
			XMFLOAT3(
				fmaxf(b1.pMax.x, b2.pMax.x),
				fmaxf(b1.pMax.y, b2.pMax.y),
				fmaxf(b1.pMax.z, b2.pMax.z)));
	}
	inline Bounds3f Intersect(const Bounds3f& b1, const Bounds3f& b2)
	{
		return Bounds3f(
			XMFLOAT3(
				fmaxf(b1.pMin.x, b2.pMin.x),
				fmaxf(b1.pMin.y, b2.pMin.y),
				fmaxf(b1.pMin.z, b2.pMin.z)),
			XMFLOAT3(
				fminf(b1.pMax.x, b2.pMax.x),
				fminf(b1.pMax.y, b2.pMax.y),
				fminf(b1.pMax.z, b2.pMax.z)));
	}
	inline bool Overlaps(const Bounds3f& b1, const Bounds3f& b2)
	{
		bool x = (b1.pMax.x >= b2.pMin.x) && (b1.pMin.x <= b2.pMax.x);
		bool y = (b1.pMax.y >= b2.pMin.y) && (b1.pMin.y <= b2.pMax.y);
		bool z = (b1.pMax.z >= b2.pMin.z) && (b1.pMin.z <= b2.pMax.z);
		return (x&&y&&z);
	}
	inline bool Inside(const XMFLOAT3& p, const Bounds3f& b)
	{
		return (
			b.pMin.x <= p.x && p.x <= b.pMax.x &&
			b.pMin.y <= p.y && p.y <= b.pMax.y &&
			b.pMin.z <= p.z && p.z <= b.pMax.z);
	}
	inline bool InsideExclusive(const XMFLOAT3& p, const Bounds3f& b)
	{
		return (
			b.pMin.x <= p.x && p.x < b.pMax.x &&
			b.pMin.y <= p.y && p.y < b.pMax.y &&
			b.pMin.z <= p.z && p.z < b.pMax.z);
	}
	inline Bounds3f Expand(const Bounds3f& b, float delta)
	{
		return Bounds3f(
			b.pMin - XMFLOAT3(delta, delta, delta),
			b.pMax + XMFLOAT3(delta, delta, delta));
	}
}
