#pragma once
#include "Collider.h"
#include "Network.h"

namespace DX {
	class QuadCollider :public Collider
	{
	public:
		QuadCollider();
		QuadCollider(XMFLOAT3 pos);

		void Visualize() override;
		bool IsHit(Geometrics::Ray ray, XMFLOAT3* hitPt) override;
		void SetScale(XMFLOAT3 s) override;
	private:

		XMFLOAT2 extent;

	};
}
