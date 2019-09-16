#include "Object.h"
#include "TextureMgr.h"
#include "Light.h"

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

	shader = new VPShader("StandardVS.cso", "StandardPS.cso", Std_ILayouts, ARRAYSIZE(Std_ILayouts));

	cb_vs_property.reset(new Buffer(sizeof(VS_Property)));
	cb_ps_dLights.reset(new Buffer(sizeof(SHADER_DIRECTIONAL_LIGHT)));
	cb_ps_pLights.reset(new Buffer(sizeof(SHADER_POINT_LIGHT)));
	cb_ps_sLights.reset(new Buffer(sizeof(SHADER_SPOT_LIGHT)));
	cb_ps_eyePos.reset(new Buffer(sizeof(XMFLOAT3)));
	cb_ps_material.reset(new Buffer(sizeof(ShaderMaterial)));

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
	r_assert(
		DX_Device->CreateSamplerState(&samplerDesc, &bodySameplerState)
	);

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
		r_assert(DX_Device->CreateShaderResourceView(mipmapTexture.Get(), &srvDesc, bodySRV.GetAddressOf()));
		DX_DContext->GenerateMips(bodySRV.Get());
	}
	else {
		bodySRV = TextureMgr::Instance()->Get(imageName);
	}

	blendState = new BlendState();
	dsState = new DepthStencilState();

	shadow_Shader = new VPShader("ShadowVS.cso", "ShadowPS.cso", Std_ILayouts, ARRAYSIZE(Std_ILayouts));
	cb_vs_shadow_property = new Buffer(sizeof(VS_Simple_Property));
	cb_ps_shadow_transparency = new Buffer(sizeof(float));
}

Object::~Object()
{
	delete transform;
	delete shape;
	delete shader;
	delete material;

	delete dsState;
	delete blendState;

	delete shadow_Shader;
	delete cb_vs_shadow_property;
	delete cb_ps_shadow_transparency;
}

void Object::EnableShadow(XMFLOAT3 shadowPlaneN, float shadowPlaneDist, float shadowTransparency)
{
	isShadow = true;
	this->shadowPlaneDist = shadowPlaneDist;
	this->shadowPlaneNormal = shadowPlaneN;
	cb_ps_shadow_transparency->Write(&shadowTransparency);
}


void Object::Update(Camera* camera, const SHADER_DIRECTIONAL_LIGHT* dLight, const SHADER_POINT_LIGHT* pLight, const SHADER_SPOT_LIGHT* sLight, const XMMATRIX& texMat)
{
	vpMat = camera->ViewMat()*camera->ProjMat(zOrder);
	XMMATRIX wMat = transform->WorldMatrix();

	cb_vs_property->Write(&VS_Property(wMat, vpMat, texMat));
	cb_ps_dLights->Write((void*)dLight);
	cb_ps_pLights->Write((void*)pLight);
	cb_ps_sLights->Write((void*)sLight);
	cb_ps_eyePos->Write(&(camera->Pos()));
	cb_ps_material->Write(material);

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
	shader->Apply();

	// TRANSFORM
	DX_DContext->VSSetConstantBuffers(0, 1, cb_vs_property->GetAddress());

	// LIGHTS
	DX_DContext->PSSetConstantBuffers(0, 1, cb_ps_dLights->GetAddress());
	DX_DContext->PSSetConstantBuffers(1, 1, cb_ps_pLights->GetAddress());
	DX_DContext->PSSetConstantBuffers(2, 1, cb_ps_sLights->GetAddress());

	// EYE
	DX_DContext->PSSetConstantBuffers(3, 1, cb_ps_eyePos->GetAddress());

	// MATERIAL
	DX_DContext->PSSetConstantBuffers(4, 1, cb_ps_material->GetAddress());

	// TEXTURE
	DX_DContext->PSSetShaderResources(0, 1, bodySRV.GetAddressOf());
	DX_DContext->PSSetSamplers(0, 1, bodySameplerState.GetAddressOf());

	// STATE
	dsState->Apply();
	blendState->Apply();
	shape->Apply();

	if (isShadow)
	{
		shadow_Shader->Apply();

		XMMATRIX wMat = transform->WorldMatrix();
		
		/*for (int i = 0; i < LIGHT_MAX_EACH; ++i)
		{
			if (DirectionalLight::Data()->enabled[i] != LIGHT_ENABLED)
				continue;

			cb_vs_shadow_property->Write(&VS_Shadow_Property(wMat*dir_light_shadowMats[i], vpMat));
			DX_DContext->VSSetConstantBuffers(0, 1, cb_vs_shadow_property->GetAddress());
			DX_DContext->PSSetConstantBuffers(0, 1, cb_ps_shadow_transparency->GetAddress());

			shape->Apply();
		}*/
		for (int i = 0; i < LIGHT_MAX_EACH; ++i)
		{
			if (PointLight::Data()->enabled[i] != LIGHT_ENABLED)
				continue;

			cb_vs_shadow_property->Write(&VS_Simple_Property(wMat*pt_light_shadowMats[i], vpMat));
			DX_DContext->VSSetConstantBuffers(0, 1, cb_vs_shadow_property->GetAddress());
			DX_DContext->PSSetConstantBuffers(0, 1, cb_ps_shadow_transparency->GetAddress());

			shape->Apply();
		}
	}
}

void Object::SetTransparency(float t)
{
	assert(0 <= t && t <= 1);

	material->SetTransparency(t);
}
