
#include "stdafx.h"
#include "Light.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

SubLight* SubLight::Create(LIGHT_KIND kind, Light* main)
{
	SubLight* newLight = nullptr;

	switch (kind)
	{
	case LIGHT_KIND::Directional:
		return new DirectionalLight(main);
	case LIGHT_KIND::Point:
		return new PointLight(main);
	case LIGHT_KIND::Spot:
		return new SpotLight(main);
	}

	return nullptr;
}

SubLight::SubLight(Light* main)
	:
	m_main(main),
	m_id(-1)
{
}


Light::Light(Object* obj)
	:
	Component(CMPNT_KIND::Light, obj)
{
}

void Light::Update(DX::Graphic* graphic)
{
}

void Light::Render(DX::Graphic* graphic) const
{
}
