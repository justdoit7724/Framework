#pragma once

#include <vector>

namespace DX {

	class Graphic;
	class Object;
	class Camera;
	class Light;
	class Buffer;
	class VShader;
	class PShader;
	class RasterizerState;
	class DepthStencilState;
	class BlendState;

	class DXLIB_DLL DepthPeeling
	{
	public:
		DepthPeeling(Graphic* graphic);
		~DepthPeeling();

		void Run(const Camera* cam, const std::vector<Light*>& lights, const std::vector<Object*>& objs, int nPic);
		void Render(ID3D11DeviceContext* dContext);
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

		BlendState* m_dxBlendState;
		RasterizerState* m_dxRState;
		DepthStencilState* m_dxDSState;
		VShader* m_dxVShader;
		PShader* m_dxPShader;

		ID3D11ShaderResourceView* m_nullSRV;
		std::vector<PictureInfo> m_vdxPic;

		Buffer* m_cbEye;
		
		Object* m_renderQuad;
	};
}

