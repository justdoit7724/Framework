#pragma once
#include "Light.h"
class PointLight : public SubLight
{
public:
	PointLight(Light* main);

	void Render(DX::Graphic* graphic)const override;

	void SetPos(XMFLOAT3 pos)override;
	void SetDir(XMFLOAT3 d) override {};
	void SetAmbient(XMFLOAT3 a) override;
	void SetDiffuse(XMFLOAT3 d) override;
	void SetSpecular(XMFLOAT3 s) override;
	void SetIntensity(float i) override;
	void Enable(bool enable) override;
	void SetAtt(XMFLOAT3 at);

private:

	struct SHADER_POINT_LIGHT {

		SHADER_POINT_LIGHT()
		{

			ZeroMemory(ambient, sizeof(XMFLOAT4) * LIGHT_MAX_EACH);
			ZeroMemory(diffuse, sizeof(XMFLOAT4) * LIGHT_MAX_EACH);
			ZeroMemory(specular, sizeof(XMFLOAT4) * LIGHT_MAX_EACH);
			ZeroMemory(pos, sizeof(XMFLOAT4) * LIGHT_MAX_EACH);
			ZeroMemory(info, sizeof(XMFLOAT4) * LIGHT_MAX_EACH);
			ZeroMemory(att, sizeof(XMFLOAT4) * LIGHT_MAX_EACH);
			ZeroMemory(intensity, sizeof(XMFLOAT4) * LIGHT_MAX_EACH);

			for (int i = 0; i < LIGHT_MAX_EACH; ++i)
			{
				info[i].x = LIGHT_DISABLED;
			}
		}

		XMFLOAT4 ambient[LIGHT_MAX_EACH];
		XMFLOAT4 diffuse[LIGHT_MAX_EACH];
		XMFLOAT4 specular[LIGHT_MAX_EACH];
		XMFLOAT4 pos[LIGHT_MAX_EACH];
		XMFLOAT4 info[LIGHT_MAX_EACH]; // enable
		XMFLOAT4 att[LIGHT_MAX_EACH];
		XMFLOAT4 intensity[LIGHT_MAX_EACH];

	};
	static SHADER_POINT_LIGHT m_data;

};

