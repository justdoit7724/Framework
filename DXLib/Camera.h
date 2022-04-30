#pragma once
#include "Component.h"


class Camera : public Component
{
public:
	Camera(Object* owner);
	~Camera();

	static void SetMain(Camera* cam);
	static const Camera* Main();

	void Update(DX::Graphic* graphic) override;
	void Render(DX::Graphic* graphic)const override;

	void SetView();
	void SetFrame(const FRAME_KIND fKind, XMFLOAT2 orthoSize, const float nearPlane, const float farPlane, const float verticalViewAngle, const float aspectRatio);
	void SetFrustum(float n, float f);

	const XMMATRIX& ProjMat()const { return m_proj; }
	void Pick(XMFLOAT2 scnPos, OUT Geom::Ray* ray)const;

	FRAME_KIND GetFrame()const { return curFrame; }
	XMFLOAT2 GetSize()const { return m_size; }
	float GetN()const { return m_near; }
	float GetF()const { return m_far; }
	float GetVRad()const { return verticalRadian; }
	float GetAspectRatio()const { return aspectRatio; }

	const Frustum* GetFrustum()const { return &frustum; }

	void SetLayer(int i)
	{
		layer = i;
	}
	void AddLayer(int i)
	{
		layer |= i;
	}
	void SubtractLayer(int i)
	{
		layer ^= i;
	}
	int Layer()const { return layer; }

private:
	XMMATRIX m_view;
	XMMATRIX m_proj;

	FRAME_KIND curFrame;
	XMFLOAT2 m_size;
	float m_near, m_far;
	float verticalRadian;
	float aspectRatio;

	Frustum frustum;
	int layer = LAYER_ALL;

};
