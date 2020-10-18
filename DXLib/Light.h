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

	class DXLIB_DLL Light
	{
	protected:
		Light();
		virtual ~Light();
		int id = -1;
		XMFLOAT3 ambient;
		XMFLOAT3 diffuse;
		XMFLOAT3 specular;

	public:

		int ID() { return id; }
		const XMFLOAT3& GetAmbient() { return ambient; }
		const XMFLOAT3& GetDiffuse() { return diffuse; }
		const XMFLOAT3& GetSpecular() { return specular; }
		virtual void SetAmbient(const XMFLOAT3& a) = 0;
		virtual void SetDiffuse(const XMFLOAT3& d) = 0;
		virtual void SetSpecular(const XMFLOAT3& s) = 0;
		virtual void SetIntensity(float i) = 0;
		virtual void Enable(bool enable) = 0;
	};

	class DXLIB_DLL DirectionalLight : public Light
	{
	private:
		static SHADER_DIRECTIONAL_LIGHT data;
		static ID3D11Buffer* cb;

	public:
		DirectionalLight(XMFLOAT3 a, XMFLOAT3 d, XMFLOAT3 s, float intensity, XMFLOAT3 dir);
		~DirectionalLight();
		void SetAmbient(const XMFLOAT3& a) override;
		void SetDiffuse(const XMFLOAT3& d) override;
		void SetSpecular(const XMFLOAT3& s) override;
		void SetIntensity(float i) override;
		void Enable(bool enable) override;

		void SetDir(XMFLOAT3 d);
		XMFLOAT3 GetDir()const;

		static void Apply(ID3D11Device* device, ID3D11DeviceContext* dContext);
	};

	class DXLIB_DLL PointLight : public Light
	{
	private:
		float range;
		XMFLOAT3 att;

		static SHADER_POINT_LIGHT data;
		static ID3D11Buffer* cb;

	public:
		PointLight(XMFLOAT3 a, XMFLOAT3 d, XMFLOAT3 s, float intensity, XMFLOAT3 att, XMFLOAT3 pos);
		~PointLight();
		void SetAmbient(const XMFLOAT3& a) override;
		void SetDiffuse(const XMFLOAT3& d) override;
		void SetSpecular(const XMFLOAT3& s) override;
		void SetIntensity(float i) override;
		void SetPos(XMFLOAT3 p);
		void SetAtt(XMFLOAT3 at);
		void Enable(bool enable) override;

		XMFLOAT3 GetPos();

		static void Apply(ID3D11Device* device, ID3D11DeviceContext* dContext);
	};

	class DXLIB_DLL SpotLight : public Light
	{
	private:
		static SHADER_SPOT_LIGHT data;
		static ID3D11Buffer* cb;

		float range;
		float spot;
		float rad;
		XMFLOAT3 att;

	public:
		SpotLight(XMFLOAT3 a, XMFLOAT3 d, XMFLOAT3 s, float range, float spot, float intensity, float rad, XMFLOAT3 att, XMFLOAT3 pos, XMFLOAT3 dir);
		~SpotLight();
		void SetAmbient(const XMFLOAT3& a) override;
		void SetDiffuse(const XMFLOAT3& d) override;
		void SetSpecular(const XMFLOAT3& s) override;
		void SetIntensity(float i) override;
		void SetPos(XMFLOAT3 p);
		void SetDir(XMFLOAT3 d);
		void SetRange(float r);
		void SetRad(float r);
		void SetSpot(float s);
		void SetAtt(XMFLOAT3 at);
		void Enable(bool enable);


		XMFLOAT3 GetPos();


		static void Apply(ID3D11Device* device, ID3D11DeviceContext* dContext);
	};

}

