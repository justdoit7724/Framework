#pragma once
#include "Component.h"

class Collider;

class SubCollider
{
public:
	static SubCollider* Create(COLLIDER_KIND kind, Collider* main);

	virtual void Update() = 0;
	virtual bool IsHit(Geom::Ray ray, XMFLOAT3* hitPt) = 0;
	virtual void SetScale(XMFLOAT3 s) = 0;
	void Translate(XMFLOAT3 pt);
	void SetRotate(XMFLOAT3 f, XMFLOAT3 u);

	const Collider* m_main;

protected:
	SubCollider(Collider* main);

	XMFLOAT3 m_center;
	XMFLOAT3 m_forward, m_up;
};

class Collider : public Component
{
public:
	Collider(Object* owner);
	~Collider();

	void Update(DX::Graphic* graphic)override;
	void Render(DX::Graphic* graphic)const override;



private:
	SubCollider* m_subCollider;

};



