#pragma once

#ifndef _GEOMETRICS
#define _GEOMETRICS

namespace Math {
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

