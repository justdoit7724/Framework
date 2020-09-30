#pragma once

namespace DX
{

class Transform;
class Camera;
class Object;
class VShader;
class BlendState;
class DepthStencilState;
class RasterizerState;
struct SHADER_DIRECTIONAL_LIGHT;
struct SHADER_POINT_LIGHT;
struct SHADER_SPOT_LIGHT;

	class Light
	{
	protected:
		Light();
		virtual ~Light();
		int id = -1;
		XMFLOAT3 ambient;
		XMFLOAT3 diffuse;
		XMFLOAT3 specular;

	public:
		enum STATE {
			ENABLED = 1,
			DISABLED = 0
		};

		int ID() { return id; }
		const XMFLOAT3& GetAmbient() { return ambient; }
		const XMFLOAT3& GetDiffuse() { return diffuse; }
		const XMFLOAT3& GetSpecular() { return specular; }
		virtual void SetAmbient(const XMFLOAT3& a) = 0;
		virtual void SetDiffuse(const XMFLOAT3& d) = 0;
		virtual void SetSpecular(const XMFLOAT3& s) = 0;
		virtual void Enable(STATE enable) = 0;
	};

	class DirectionalLight : public Light
	{
	private:
		static SHADER_DIRECTIONAL_LIGHT data;
		static ID3D11Buffer* cb;

	public:
		DirectionalLight(XMFLOAT3 a, XMFLOAT3 d, XMFLOAT3 s, XMFLOAT3 dir);
		~DirectionalLight();
		void SetAmbient(const XMFLOAT3& a) override;
		void SetDiffuse(const XMFLOAT3& d) override;
		void SetSpecular(const XMFLOAT3& s) override;
		void SetDir(XMFLOAT3 d);
		void Enable(STATE enable) override;

		XMFLOAT3 GetDir()const;

		static void Apply();
	};

	class PointLight : public Light
	{
	private:
		float range;
		XMFLOAT3 att;

		static SHADER_POINT_LIGHT data;
		static ID3D11Buffer* cb;

	public:
		PointLight(XMFLOAT3 a, XMFLOAT3 d, XMFLOAT3 s, float range, XMFLOAT3 att, XMFLOAT3 pos);
		~PointLight();
		void SetAmbient(const XMFLOAT3& a) override;
		void SetDiffuse(const XMFLOAT3& d) override;
		void SetSpecular(const XMFLOAT3& s) override;
		void SetPos(XMFLOAT3 p);
		void SetRange(float r);
		void SetAtt(XMFLOAT3 at);
		void Enable(STATE enable) override;

		static void Apply();
	};

	class SpotLight : public Light
	{
	private:
		static SHADER_SPOT_LIGHT data;
		static ID3D11Buffer* cb;

		float range;
		float spot;
		float rad;
		XMFLOAT3 att;

	public:
		SpotLight(XMFLOAT3 a, XMFLOAT3 d, XMFLOAT3 s, float range, float spot, float rad, XMFLOAT3 att, XMFLOAT3 pos, XMFLOAT3 dir);
		~SpotLight();
		void SetAmbient(const XMFLOAT3& a) override;
		void SetDiffuse(const XMFLOAT3& d) override;
		void SetSpecular(const XMFLOAT3& s) override;
		void SetPos(XMFLOAT3 p);
		void SetDir(XMFLOAT3 d);
		void SetRange(float r);
		void SetRad(float r);
		void SetSpot(float s);
		void SetAtt(XMFLOAT3 at);
		void Enable(STATE enable);

		static void Apply();
	};

}

