#pragma once
#include "Network.h"

#include "Geometrics.h"

namespace DX
{
	class DXLIB_DLL Collider : public IDebug
	{
	public:
		Collider(XMFLOAT3 center);

		virtual bool IsHit(Geometrics::Ray ray, XMFLOAT3* hitPt) = 0;

		void Translate(XMFLOAT3 pt) { center = pt; }
		void SetRotate(XMFLOAT3 f, XMFLOAT3 u);
		virtual void SetScale(XMFLOAT3 s) = 0;

	protected:

		XMFLOAT3 center;
		XMFLOAT3 forward, up;
	};
}


