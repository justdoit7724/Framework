#pragma once
#include "Collider.h"

namespace DX
{
	class CubeCollider : public Collider
	{
	public:
		CubeCollider();
		CubeCollider(XMFLOAT3 center, XMFLOAT3 extent);

		bool IsHit(Geometrics::Ray ray, XMFLOAT3* hitPt) override;
		void SetScale(XMFLOAT3 s) override;
		void Visualize() override;

	private:

		XMFLOAT3 extent;
	};
}