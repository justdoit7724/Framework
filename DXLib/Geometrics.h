#pragma once

#ifndef _GEOMETRICS
#define _GEOMETRICS

#include "Math.h"

namespace DX {

	namespace Geometrics {

		struct Sphere {
		public:
			XMFLOAT3 p;
			float rad;

			Sphere() :p(XMFLOAT3(0, 0, 0)), rad(1) {}
			Sphere(XMFLOAT3 p, float rad) :p(p), rad(rad) {}
		};
		struct PlaneInf {
		public:
			XMFLOAT3 p;
			XMFLOAT3 n;
			PlaneInf() :p(XMFLOAT3(0, 0, 0)), n(XMFLOAT3(0, 0, 0)) {}
			PlaneInf(XMFLOAT3 p, XMFLOAT3 n) :p(p), n(n) {}
		};
		struct Plane {
		public:
			XMFLOAT3 c;
			XMFLOAT3 normal;
			XMFLOAT3 up;
			XMFLOAT3 right;
			XMFLOAT2 rad;
			Plane() :c(XMFLOAT3(0, 0, 0)), normal(XMFLOAT3(0, 0, 0)), up(XMFLOAT3(0, 0, 0)), right(XMFLOAT3(0, 0, 0)), rad(XMFLOAT2(0, 0)) {}
			Plane(XMFLOAT3 c, XMFLOAT3 normal, XMFLOAT3 up, XMFLOAT2 rad) :c(c), normal(normal), up(up), right(Cross(up, normal)), rad(rad) {}
		};
		struct Bound {
		public:
			XMFLOAT3 center;
			XMFLOAT3 rad;
			Bound() : center(XMFLOAT3(0, 0, 0)), rad(XMFLOAT3(0, 0, 0)) {}
			Bound(XMFLOAT3 c, XMFLOAT3 rad) :center(c), rad(rad) {}
		};
		struct Ray {

		public:
			XMFLOAT3 o;
			XMFLOAT3 d;

			Ray() :o(XMFLOAT3(0, 0, 0)), d(XMFLOAT3(0, 0, 0)) {}
			Ray(const XMFLOAT3& o, const XMFLOAT3& d) : o(o), d(d) {}

			XMFLOAT3 operator() (float t) const {

				return o + d * t;
			}
		};
		struct Ray2D {
		public:
			XMFLOAT2 p;
			XMFLOAT2 d;

			Ray2D() {
				p = XMFLOAT2(0, 0);
				d = XMFLOAT2(0, 1);
			}
			Ray2D(XMFLOAT2 pt, XMFLOAT2 dir)
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
			XMFLOAT3 toRayPt = ray.o - sph.p;
			XMFLOAT3 tempRight = Normalize(Cross(ray.d, toRayPt));
			XMFLOAT3 dir = Cross(tempRight, ray.d);

			return (Dot(toRayPt, dir) < sph.rad);
		}
		inline bool DXLIB_DLL IntersectRayPlaneInf(const Ray ray, const PlaneInf plane, XMFLOAT3* itsPt)
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
				*itsPt = pt;

			return (
				(Dot(ray.d, pt - ray.o) > 0) && // check if ray is ahead of plane
				(xDist < plane.rad.x) &&  // check if hitPt is in x range
				(yDist < plane.rad.y) // check if hitPt is in y range
				);
		}
		inline bool Intersect2DRayLine(Ray2D ray, XMFLOAT2 linePt1, XMFLOAT2 linePt2)
		{
			XMFLOAT2 o = ray.p;
			XMFLOAT2 d = ray.d;
			XMFLOAT2 a = linePt1;
			XMFLOAT2 b = linePt2;

			if (a == b || d == XMFLOAT2(0,0))
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

			return (t1>0&&0<t2&&t2<=1);
		}
	}
}

#endif

