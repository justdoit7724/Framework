#pragma once
#include "Component.h"
#include "Math.h"

#define LIGHT_ENABLED 1
#define LIGHT_DISABLED 0
#define LIGHT_MAX_EACH 3

class Light;

class SubLight
{
public:
	static SubLight* Create(LIGHT_KIND kind, Light* main);

	virtual void Render(DX::Graphic* graphic)const = 0;

	virtual void SetPos(XMFLOAT3 pos) = 0;
	virtual void SetDir(XMFLOAT3 d) = 0;
	virtual void SetAmbient(XMFLOAT3 a) = 0;
	virtual void SetDiffuse(XMFLOAT3 d) = 0;
	virtual void SetSpecular(XMFLOAT3 s) = 0;
	virtual void SetIntensity(float i) = 0;
	virtual void Enable(bool enable) = 0;

	const Light* m_main;

protected:
	SubLight(Light* main);
	int m_id;
};

class Light : public Component
{
public:

	Light(Object* owner);

	

	// Inherited via Component
	virtual void Update(DX::Graphic* graphic) override;

	virtual void Render(DX::Graphic* graphic) const override;

};



