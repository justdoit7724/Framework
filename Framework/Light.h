#pragma once
#include "DX_info.h"

class Transform;
class Camera;
struct SHADER_DIRECTIONAL_LIGHT;
struct SHADER_POINT_LIGHT;
struct SHADER_SPOT_LIGHT;

class Light
{
protected:
	int id=-1;
	XMFLOAT3 ambient;
	XMFLOAT3 diffuse;
	XMFLOAT3 specular;
	Light(XMFLOAT3 a, XMFLOAT3 d, XMFLOAT3 s);
	~Light();

public:
	int ID() {return id;}
	const XMFLOAT3& GetAmbient(){return ambient;}
	const XMFLOAT3& GetDiffuse(){return diffuse;}
	const XMFLOAT3& GetSpecular(){return specular;}
	virtual void SetAmbient(const XMFLOAT3& a)=0;
	virtual void SetDiffuse(const XMFLOAT3& d)=0;
	virtual void SetSpecular(const XMFLOAT3& s)=0;
};

class DirectionalLight : public Light
{
private:
	static SHADER_DIRECTIONAL_LIGHT data;
	static ID3D11Buffer* cb;
	Camera* view;

public:
	DirectionalLight(XMFLOAT3 a, XMFLOAT3 d, XMFLOAT3 s, XMFLOAT3 dir);
	void SetAmbient(const XMFLOAT3& a) override;
	void SetDiffuse(const XMFLOAT3& d) override;
	void SetSpecular(const XMFLOAT3& s) override;
	void SetDir( XMFLOAT3 d); 

	static void Apply();
};

class PointLight : public Light
{
private:
	static SHADER_POINT_LIGHT data;
	static ID3D11Buffer* cb;
	float range;
	XMFLOAT3 att;
	Camera* view[6];

public:
	PointLight(XMFLOAT3 a, XMFLOAT3 d, XMFLOAT3 s, float range, XMFLOAT3 att);
	void SetAmbient(const XMFLOAT3& a) override;
	void SetDiffuse(const XMFLOAT3& d) override;
	void SetSpecular(const XMFLOAT3& s) override;
	void SetPos(XMFLOAT3 p);
	void SetRange( float r);
	void SetAtt( XMFLOAT3 at);

	static void Apply();
};

class SpotLight : public Light
{
private:
	static SHADER_SPOT_LIGHT data;
	static ID3D11Buffer* cb;
	float range;
	float spot;
	XMFLOAT3 att;
	Camera* view;
public:
	SpotLight(XMFLOAT3 a, XMFLOAT3 d, XMFLOAT3 s,XMFLOAT3 pos, XMFLOAT3 dir, float range, float spot, XMFLOAT3 att);
	void SetAmbient(const XMFLOAT3& a) override;
	void SetDiffuse(const XMFLOAT3& d) override;
	void SetSpecular(const XMFLOAT3& s) override;
	void SetPos( XMFLOAT3 p);
	void SetDir( XMFLOAT3 d);
	void SetRange( float r);
	void SetSpot( float s);
	void SetAtt( XMFLOAT3 at);

	static void Apply();
};



