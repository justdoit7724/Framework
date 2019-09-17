#include "Object.h"
#include "TextureMgr.h"
#include "Light.h"
#include "Shader.h"

int CalculateMaxMiplevel(int width, int height)
{
	int maxResol = max(width, height);
	int maxMiplevel = 1;

	while (maxResol > 1)
	{
		maxResol = floor(maxResol * 0.5f);
		maxMiplevel++;
	}

	return maxMiplevel;
}

Object::Object(Shape* shape, XMFLOAT3 mDiffuse, XMFLOAT3 mAmbient, XMFLOAT3 mSpec, float sP, XMFLOAT3 r, std::string imageName, bool mipmap, int zOrder)
	: isMipmap(mipmap), zOrder(zOrder), shape(shape)
{
	transform = new Transform();

	vs = new VShader("StandardVS.cso", Std_ILayouts, ARRAYSIZE(Std_ILayouts));
	ps = new PShader("StandardPS.cso");

	vs->AddCB(0, 1, sizeof(VS_Property));
	ps->AddCB(0, 1, sizeof(SHADER_DIRECTIONAL_LIGHT));
	ps->AddCB(1, 1, sizeof(SHADER_POINT_LIGHT));
	ps->AddCB(2, 1, sizeof(SHADER_SPOT_LIGHT));
	ps->AddCB(3, 1, sizeof(XMFLOAT3));
	ps->AddCB(4, 1, sizeof(ShaderMaterial));
	
	material = new ShaderMaterial(mDiffuse, 1, mAmbient, mSpec, sP, r);

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	ps->AddSamp(0, 1, &samplerDesc);


	if (mipmap)
	{
		TextureMgr::Instance()->Load(imageName);
		ID3D11Texture2D* image = TextureMgr::Instance()->GetTexture(imageName);
		ComPtr<ID3D11Texture2D> mipmapTexture = nullptr;
		D3D11_TEXTURE2D_DESC imageDesc;
		image->GetDesc(&imageDesc);

		D3D11_TEXTURE2D_DESC texDesc;
		texDesc.ArraySize = 1;
		texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		texDesc.CPUAccessFlags = 0;
		texDesc.Format = imageDesc.Format;
		texDesc.Width = imageDesc.Width;
		texDesc.Height = imageDesc.Height;
		texDesc.MipLevels = CalculateMaxMiplevel(imageDesc.Width, imageDesc.Height);
		texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.SampleDesc = { 1,0 };
		r_assert(DX_Device->CreateTexture2D(&texDesc, nullptr, mipmapTexture.GetAddressOf()));

		ComPtr<ID3D11Texture2D> stagTex;
		D3D11_TEXTURE2D_DESC stagDesc;
		stagDesc.ArraySize = 1;
		stagDesc.BindFlags = 0;
		stagDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		stagDesc.Format = imageDesc.Format;
		stagDesc.Width = imageDesc.Width;
		stagDesc.Height = imageDesc.Height;
		stagDesc.MipLevels = 1;
		stagDesc.MiscFlags = 0;
		stagDesc.Usage = D3D11_USAGE_STAGING;
		stagDesc.SampleDesc = { 1,0 };
		r_assert(DX_Device->CreateTexture2D(&stagDesc, nullptr, &stagTex));

		DX_DContext->CopyResource(stagTex.Get(), image);

		D3D11_MAPPED_SUBRESOURCE mapped;
		r_assert(DX_DContext->Map(stagTex.Get(), 0, D3D11_MAP_READ, 0, &mapped));
		UINT* arr = new UINT[(mapped.RowPitch / (float)sizeof(UINT)) * imageDesc.Height];
		ZeroMemory(arr, mapped.RowPitch*imageDesc.Height);
		CopyMemory(arr, mapped.pData, mapped.RowPitch*imageDesc.Height);
		DX_DContext->Unmap(stagTex.Get(), 0);

		DX_DContext->UpdateSubresource(mipmapTexture.Get(), 0, &CD3D11_BOX(0, 0, 0, imageDesc.Width, imageDesc.Height, 1), arr, mapped.RowPitch, mapped.DepthPitch);
		delete[] arr;

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = imageDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;
		ID3D11ShaderResourceView* bodySrv = nullptr;
		r_assert(DX_Device->CreateShaderResourceView(mipmapTexture.Get(), &srvDesc, &bodySrv));
		DX_DContext->GenerateMips(bodySrv);
		ps->AddSRV(0, 1, bodySrv);
	}
	else {
		ps->AddSRV(0, 1, TextureMgr::Instance()->Get(imageName));
	}

	blendState = new BlendState();
	dsState = new DepthStencilState();

	shadow_vs = new VShader("ShadowVS.cso", Std_ILayouts, ARRAYSIZE(Std_ILayouts));
	shadow_ps = new PShader("ShadowPS.cso");
	shadow_vs->AddCB(0, 1, sizeof(VS_Simple_Property));
	shadow_ps->AddCB(0, 1, sizeof(float));
}

Object::~Object()
{
	delete transform;
	delete shape;
	delete vs;
	delete ps;
	delete material;

	delete dsState;
	delete blendState;

	delete shadow_vs;
	delete shadow_ps;
}

void Object::EnableShadow(XMFLOAT3 shadowPlaneN, float shadowPlaneDist, float shadowTransparency)
{
	isShadow = true;
	this->shadowPlaneDist = shadowPlaneDist;
	this->shadowPlaneNormal = shadowPlaneN;
	shadow_ps->WriteCB(0,&shadowTransparency);
}


void Object::Update(Camera* camera, const SHADER_DIRECTIONAL_LIGHT* dLight, const SHADER_POINT_LIGHT* pLight, const SHADER_SPOT_LIGHT* sLight, const XMMATRIX& texMat)
{
	vpMat = camera->ViewMat()*camera->ProjMat(zOrder);
	XMMATRIX wMat = transform->WorldMatrix();

	vs->WriteCB(0,&VS_Property(wMat, vpMat, texMat));
	ps->WriteCB(0, (void*)dLight);
	ps->WriteCB(1, (void*)pLight);
	ps->WriteCB(2, (void*)sLight);
	ps->WriteCB(3, &(camera->Pos()));
	ps->WriteCB(4, material);

	if (isShadow)
	{
		/*for (int i = 0; i < LIGHT_MAX_EACH; ++i)
		{
			if (DirectionalLight::Data()->enabled[i] != LIGHT_ENABLED)
				continue;

			XMFLOAT3 n = shadowPlaneNormal;
			float d = shadowPlaneDist;
			XMFLOAT3 l = XMFLOAT3(
				DirectionalLight::Data()->dir[i].x,
				DirectionalLight::Data()->dir[i].y,
				DirectionalLight::Data()->dir[i].z);
			float nl = Dot(n, l);
			dir_light_shadowMats[i] = XMMATRIX(
				nl - l.x*n.x, -l.y*n.x, -l.z*n.x, 0,
				-l.x*n.y, nl - l.y*n.y, -l.z*n.y, 0,
				-l.x*n.z, -l.y*n.z, nl - l.z*n.z, 0,
				l.x*d, l.y*d, l.z*d, nl
			);
		}*/
		for (int i = 0; i < LIGHT_MAX_EACH; ++i)
		{
			if (PointLight::Data()->enabled[i] != LIGHT_ENABLED)
				continue;

			XMFLOAT3 n = shadowPlaneNormal;
			float d = shadowPlaneDist;
			XMFLOAT3 l = XMFLOAT3(
				PointLight::Data()->pos[i].x,
				PointLight::Data()->pos[i].y,
				PointLight::Data()->pos[i].z);
			float nl = Dot(n, l);
			pt_light_shadowMats[i] = -XMMATRIX(
				nl - d - l.x*n.x,	-l.y*n.x,			-l.z*n.x,			-n.x,
				-l.x*n.y,			nl + d - l.y*n.y,	-l.z*n.y,			-n.y,
				-l.x*n.z,			-l.y*n.z,			nl + d - l.z*n.z,	-n.z,
				l.x*d,				l.y*d,				l.z*d,				nl
			);
		}
	}

	
}

void Object::Render()
{
	vs->Apply();
	ps->Apply();

	// STATE
	dsState->Apply();
	blendState->Apply();
	shape->Apply();

	if (isShadow)
	{
		XMMATRIX wMat = transform->WorldMatrix();
		
		for (int i = 0; i < LIGHT_MAX_EACH; ++i)
		{
			if (DirectionalLight::Data()->enabled[i] != LIGHT_ENABLED)
				continue;

			shadow_vs->WriteCB(0,&VS_Simple_Property(wMat*dir_light_shadowMats[i], vpMat));
			shadow_vs->Apply();
			shadow_ps->Apply();

			shape->Apply();
		}
		for (int i = 0; i < LIGHT_MAX_EACH; ++i)
		{
			if (PointLight::Data()->enabled[i] != LIGHT_ENABLED)
				continue;

			shadow_vs->WriteCB(0,&VS_Simple_Property(wMat*pt_light_shadowMats[i], vpMat));
			shadow_vs->Apply();
			shadow_ps->Apply();

			shape->Apply();
		}
	}
}

void Object::SetTransparency(float t)
{
	assert(0 <= t && t <= 1);

	material->SetTransparency(t);
}
