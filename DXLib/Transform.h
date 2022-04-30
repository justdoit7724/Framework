#pragma once

#include "Component.h"

class Transform : public Component
{
public:
	Transform(Object* owner);

	XMMATRIX WorldMatrix()const;
	XMMATRIX S()const;
	XMMATRIX R()const;
	XMMATRIX T()const;

	XMFLOAT3 GetPos()const { return m_pos; }
	XMFLOAT3 GetForward()const {return m_forward;}
	XMFLOAT3 GetUp()const{return m_up;}
	XMFLOAT3 GetRight()const{return m_right;}
	XMFLOAT3 GetScale()const{return m_scale;}
	void SetTranslation(float x, float y, float z) {m_pos = { x,y,z };}
	void SetTranslation(XMFLOAT3 newPos) {m_pos = newPos;}
	void SetRot(XMFLOAT3 _forward);
	void SetRot(XMFLOAT3 _forward, XMFLOAT3 _up);
	void SetRot(XMFLOAT3 _forward, XMFLOAT3 _up, XMFLOAT3 _right);
	void Rotate(XMFLOAT3 axis, float rad);
	void SetScale(float x) {m_scale = { x,x,x };}
	void SetScale(float x, float y, float z) {m_scale = { x,y,z };}
	void SetScale(XMFLOAT3 newScale) {m_scale = newScale;}

	void Update(DX::Graphic* graphic) override;
	void Render(DX::Graphic* graphic)const override;
private:

	XMFLOAT3 m_pos;
	XMFLOAT3 m_forward;
	XMFLOAT3 m_right;
	XMFLOAT3 m_up;
	XMFLOAT3 m_scale;
	XMMATRIX m_world;
};


