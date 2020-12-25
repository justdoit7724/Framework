#pragma once

#include <vector>

namespace DX {

	class Graphic;
	class Object;
	class Camera;
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

		void Run(const Camera* cam, const std::vector<Object*>& objs, int nPic);
		void ApplyDSV(int index);
		ID3D11ShaderResourceView* GetSRV(int index);
		ID3D11DepthStencilView* GetDSV(int index);

	private:
		struct PictureInfo
		{
			ID3D11DepthStencilView* dsv;
			ID3D11ShaderResourceView* srv;
		};
		void Create(PictureInfo* picInfo);
		void PreRun();

		Graphic* m_dxGraphic;

		BlendState* m_dxBlendState;
		RasterizerState* m_dxRState;
		DepthStencilState* m_dxDSState;
		VShader* m_dxVShader;
		PShader* m_dxPShader;

		ID3D11RenderTargetView* m_dxRTV;
		std::vector<PictureInfo> m_vdxPic;
	};
}

