#pragma once


namespace DX
{
	class VShader;
	class PShader;
	class Mesh;
	class BlendState;
	class DepthStencilState;
	class RasterizerState;

	class PostProcessing
	{
	public:
		PostProcessing(ID3D11Device* device, std::string psPath);
		~PostProcessing();

		void Render(ID3D11DeviceContext* dContext, ID3D11RenderTargetView* rtv=nullptr);

	private:

		VShader* m_vs;
		PShader* m_ps;
		Mesh* m_screenMesh;

		BlendState* m_blendState;
		DepthStencilState* m_dsState;
		RasterizerState* m_rState;
	};
}

