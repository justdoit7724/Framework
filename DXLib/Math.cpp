#include "pch.h"
#include "Math.h"

namespace DX {

	DXLIB_DLL XMFLOAT2 operator+(XMFLOAT2 a, XMFLOAT2 b)
	{
		return XMFLOAT2(a.x + b.x, a.y + b.y);
	}

	DXLIB_DLL XMFLOAT2 operator-(XMFLOAT2 a, XMFLOAT2 b)
	{
		return XMFLOAT2(a.x - b.x, a.y - b.y);
	}

	DXLIB_DLL XMFLOAT2 operator+(XMFLOAT2 v, float f)
	{
		return XMFLOAT2(v.x + f, v.y + f);
	}

	DXLIB_DLL XMFLOAT2 operator-(XMFLOAT2 v, float f)
	{
		return XMFLOAT2(v.x - f, v.y - f);
	}

	DXLIB_DLL XMFLOAT2 operator*(XMFLOAT2 v, float f)
	{
		return XMFLOAT2(v.x * f, v.y * f);
	}

	DXLIB_DLL XMFLOAT2 operator/(XMFLOAT2 v, float f)
	{
		return XMFLOAT2(v.x / f, v.y / f);
	}

	DXLIB_DLL XMFLOAT3 operator+(XMFLOAT3 a, XMFLOAT3 b)
	{
		XMFLOAT3 sum;
		sum.x = a.x + b.x;
		sum.y = a.y + b.y;
		sum.z = a.z + b.z;
		return sum;
	}

	DXLIB_DLL void operator+=(XMFLOAT3& a, XMFLOAT3 b)
	{
		a.x += b.x;
		a.y += b.y;
		a.z += b.z;
	}

	DXLIB_DLL XMFLOAT3 operator-(XMFLOAT3 a, XMFLOAT3 b)
	{
		XMFLOAT3 sum;
		sum.x = a.x - b.x;
		sum.y = a.y - b.y;
		sum.z = a.z - b.z;
		return sum;
	}

	DXLIB_DLL XMFLOAT3 operator-(XMFLOAT3 v)
	{
		XMFLOAT3 sum;
		sum.x = -v.x;
		sum.y = -v.y;
		sum.z = -v.z;
		return sum;
	}

	DXLIB_DLL XMFLOAT3 operator+(XMFLOAT3 v, float f)
	{
		XMFLOAT3 sum;
		sum.x = v.x + f;
		sum.y = v.y + f;
		sum.z = v.z + f;
		return sum;
	}

	DXLIB_DLL XMFLOAT3 operator-(XMFLOAT3 v, float f)
	{
		XMFLOAT3 sum;
		sum.x = v.x - f;
		sum.y = v.y - f;
		sum.z = v.z - f;
		return sum;
	}

	DXLIB_DLL XMFLOAT3 operator*(XMFLOAT3 v, float f)
	{
		XMFLOAT3 sum;
		sum.x = v.x * f;
		sum.y = v.y * f;
		sum.z = v.z * f;
		return sum;
	}

	DXLIB_DLL XMFLOAT3 operator*(XMFLOAT3 a, XMFLOAT3 b)
	{
		XMFLOAT3 sum;
		sum.x = a.x * b.x;
		sum.y = a.y * b.y;
		sum.z = a.z * b.z;
		return sum;
	}

	DXLIB_DLL XMFLOAT3 operator/(XMFLOAT3 v, float f)
	{
		XMFLOAT3 sum;
		sum.x = v.x / f;
		sum.y = v.y / f;
		sum.z = v.z / f;
		return sum;
	}

	DXLIB_DLL void operator/=(XMFLOAT4& v, float f)
	{
		v.x /= f;
		v.y /= f;
		v.z /= f;
		v.w /= f;
	}

	DXLIB_DLL bool operator==(XMFLOAT2 a, XMFLOAT2 b)
	{
		return (a.x == b.x && a.y == b.y);
	}

	DXLIB_DLL bool operator!=(XMFLOAT2 a, XMFLOAT2 b)
	{
		return (a.x != b.x || a.y != b.y);
	}

	DXLIB_DLL bool operator==(XMFLOAT3 a, XMFLOAT3 b)
	{
		return (a.x == b.x && a.y == b.y && a.z == b.z);
	}

	DXLIB_DLL bool operator!=(XMFLOAT3 a, XMFLOAT3 b)
	{
		return (a.x != b.x || a.y != b.y || a.z != b.z);
	}

	DXLIB_DLL bool operator==(XMFLOAT4 a, XMFLOAT4 b)
	{
		return (a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w);
	}

	DXLIB_DLL bool operator!=(XMFLOAT4 a, XMFLOAT4 b)
	{
		return (a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w);
	}

	DXLIB_DLL XMFLOAT4 operator*(XMFLOAT4 v, float f)
	{
		return XMFLOAT4(v.x * f, v.y * f, v.z * f, v.w * f);
	}

	DXLIB_DLL float Clamp(float a, float b, float v)
	{
		return fmaxf(a, fminf(v, b));
	}

	DXLIB_DLL float Lerp(float v1, float v2, float t)
	{
		return v1 + (v2 - v1) * Clamp(0, 1, t);
	}

	DXLIB_DLL float Rand01()
	{
		return (rand() / (float)RAND_MAX);
	}

	DXLIB_DLL float Length(XMFLOAT2 v)
	{
		return sqrtf(v.x * v.x + v.y * v.y);
	}

	DXLIB_DLL XMFLOAT2 Cross(XMFLOAT2 a, XMFLOAT2 b)
	{
		XMVECTOR v = XMVector2Cross(XMLoadFloat2(&a), XMLoadFloat2(&b));
		XMFLOAT2 f2;
		XMStoreFloat2(&f2, v);
		return f2;
	}

	DXLIB_DLL float Dot(XMFLOAT2 a, XMFLOAT2 b)
	{
		XMVECTOR v = XMVector2Dot(XMLoadFloat2(&a), XMLoadFloat2(&b));
		XMFLOAT2 f2;
		XMStoreFloat2(&f2, v);
		return f2.x;
	}

	DXLIB_DLL XMFLOAT3 Multiply(XMFLOAT3 v, const XMMATRIX& m)
	{
		XMVECTOR mv = XMVector3Transform(XMLoadFloat3(&v), m);
		XMFLOAT3 f3;
		XMStoreFloat3(&f3, mv);
		return f3;
	}

	DXLIB_DLL XMFLOAT3 MultiplyDir(XMFLOAT3 v, const XMMATRIX& m)
	{
		XMVECTOR mv = XMVector3TransformNormal(XMLoadFloat3(&v), m);
		XMFLOAT3 f3;
		XMStoreFloat3(&f3, mv);
		return f3;
	}

	DXLIB_DLL float Length(XMFLOAT3 v)
	{
		return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	}

	DXLIB_DLL XMFLOAT3 Cross(XMFLOAT3 a, XMFLOAT3 b)
	{
		XMVECTOR v = XMVector3Cross(XMLoadFloat3(&a), XMLoadFloat3(&b));
		XMFLOAT3 f3;
		XMStoreFloat3(&f3, v);
		return f3;
	}

	DXLIB_DLL XMFLOAT3 Lerp(XMFLOAT3 v1, XMFLOAT3 v2, float t)
	{
		return v1 + (v2 - v1) * Clamp(0, 1, t);
	}

	DXLIB_DLL float Dot(XMFLOAT3 a, XMFLOAT3 b)
	{
		XMVECTOR v = XMVector3Dot(XMLoadFloat3(&a), XMLoadFloat3(&b));
		XMFLOAT3 f3;
		XMStoreFloat3(&f3, v);
		return f3.x;
	}

	DXLIB_DLL XMFLOAT3 Normalize(XMFLOAT3 v)
	{
		XMVECTOR nv = XMVector3Normalize(XMLoadFloat3(&v));
		XMFLOAT3 f3;
		XMStoreFloat3(&f3, nv);
		return f3;
	}

	DXLIB_DLL XMFLOAT3 Abs(XMFLOAT3 v)
	{
		XMFLOAT3 sum;
		sum.x = abs(v.x);
		sum.y = abs(v.y);
		sum.z = abs(v.z);
		return sum;
	}

	DXLIB_DLL float Max(XMFLOAT3 v)
	{
		return XMMax(v.x, XMMax(v.y, v.z));
	}

	DXLIB_DLL float Rad(XMFLOAT3 d1, XMFLOAT3 d2)
	{
		return acosf(Dot(d1, d2));
	}

	DXLIB_DLL XMFLOAT3 RotateFromDir(XMFLOAT3 p, XMFLOAT3 dir, float rad)
	{
		XMFLOAT3 c = dir * Dot(dir, p);
		XMFLOAT3 right = p - c;
		XMFLOAT3 up = Cross(dir, right);

		return c + right * cosf(rad) + up * sinf(rad);
	}

	DXLIB_DLL XMFLOAT4 Multiply(XMFLOAT4 v, const XMMATRIX& m)
	{
		XMVECTOR mv = XMVector4Transform(XMLoadFloat4(&v), m);
		XMFLOAT4 f4;
		XMStoreFloat4(&f4, mv);
		return f4;
	}
}