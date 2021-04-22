#pragma once
#include "Light.h"
class DirectionalLight : public SubLight
{
public:
	DirectionalLight(Light* main);

	void Render(DX::Graphic* graphic)const override;

	void SetPos(XMFLOAT3 pos)override {};
	void SetDir(XMFLOAT3 d) override;
	void SetAmbient(XMFLOAT3 a) override;
	void SetDiffuse(XMFLOAT3 d) override;
	void SetSpecular(XMFLOAT3 s) override;
	void SetIntensity(float i) override;
	void Enable(bool enable) override;

private:

	struct SHADER_DIRECTIONAL_LIGHT {

		SHADER_DIRECTIONAL_LIGHT() {

			ZeroMemory(ambient, sizeof(XMFLOAT4) * LIGHT_MAX_EACH);
			ZeroMemory(diffuse, sizeof(XMFLOAT4) * LIGHT_MAX_EACH);
			ZeroMemory(specular, sizeof(XMFLOAT4) * LIGHT_MAX_EACH);
			ZeroMemory(dir, sizeof(XMFLOAT4) * LIGHT_MAX_EACH);
			ZeroMemory(enabled, sizeof(XMFLOAT4) * LIGHT_MAX_EACH);
			ZeroMemory(intensity, sizeof(XMFLOAT4) * LIGHT_MAX_EACH);

			for (int i = 0; i < LIGHT_MAX_EACH; ++i)
			{
				enabled[i] = XMFLOAT4(LIGHT_DISABLED, 0, 0, 0);
			}
		}

		XMFLOAT4 ambient[LIGHT_MAX_EACH];
		XMFLOAT4 diffuse[LIGHT_MAX_EACH];
		XMFLOAT4 specular[LIGHT_MAX_EACH];
		XMFLOAT4 dir[LIGHT_MAX_EACH];
		XMFLOAT4 enabled[LIGHT_MAX_EACH]; // enable
		XMFLOAT4 intensity[LIGHT_MAX_EACH];

	};
	static SHADER_DIRECTIONAL_LIGHT m_data;

};

