#pragma once

class Camera;
class Light;
class Object;

class DepthPeeling
{
public:
	DepthPeeling(DX::Graphic* graphic);
	~DepthPeeling();

	void Run(const Camera* cam, const std::vector<Light*>& lights, const std::vector<Object*>& objs, int nPic);
	/*
	void Render(ID3D11DeviceContext* dContext, int iIndex = -1);
	void GetSRV(int index, ID3D11ShaderResourceView** ppSRV);
	void GetDSV(int index, ID3D11DepthStencilView** ppDSV);
	void GetRTV(int index, ID3D11RenderTargetView** ppRTV);

private:
	struct PictureInfo
	{
		ID3D11DepthStencilView* dsv;
		ID3D11ShaderResourceView* depthSrv;
		ID3D11RenderTargetView* rtv;
		ID3D11ShaderResourceView* backSrv;
	};
	void Create(PictureInfo* picInfo);
	void PreRun(int count);

	Graphic* m_dxGraphic;

	VShader* m_dxVShader;
	PShader* m_dxPShader;

	std::vector<PictureInfo> m_vdxPic;

	Object* m_renderQuad;
	*/
};


