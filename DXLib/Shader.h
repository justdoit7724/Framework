#pragma once

#include "Component.h"

namespace DX {


	class Buffer;

	class DXLIB_DLL Shader : public Component
	{
	public:
		~Shader();
		void AddCB(ID3D11Device* device, UINT slot, UINT arrayNum, UINT byteSize);
		void AddSRV(UINT slot, UINT arrayNum);
		void AddSamp(ID3D11Device* device, UINT slot, UINT arrayNum, D3D11_SAMPLER_DESC* desc);
		void WriteCB(ID3D11DeviceContext* dContext, UINT slot, const void* data);
		void WriteSRV(UINT slot, ID3D11ShaderResourceView* srv);
		void RemoveCB(UINT slot);
		bool CheckCBSlot(UINT slot);

		virtual void UnboundSRVAll(ID3D11DeviceContext* dContext)const = 0;

	protected:
		struct BindingCB
		{
			Buffer* data;
			UINT arrayNum;
			BindingCB()
				:data(nullptr), arrayNum(0) {}
			BindingCB(Buffer* data, UINT arrayNum)
				:data(data), arrayNum(arrayNum) {}
		};
		struct BindingSRV
		{
			ID3D11ShaderResourceView* data;
			UINT arrayNum;
			BindingSRV()
				:data(nullptr), arrayNum(0) {}
			BindingSRV(ID3D11ShaderResourceView* data, UINT arrayNum)
				:data(data), arrayNum(arrayNum) {}
		};
		struct BindingSamp
		{
			ID3D11SamplerState* data;
			UINT arrayNum;
			BindingSamp()
				:data(nullptr), arrayNum(0) {}
			BindingSamp(ID3D11SamplerState* data, UINT arrayNum)
				:data(data), arrayNum(arrayNum) {}
		};

		std::unordered_map<UINT, BindingCB> cbs;
		std::unordered_map<UINT, BindingSRV> srvs;
		std::unordered_map<UINT, BindingSamp> samps;
	};

	class DXLIB_DLL VShader : public Shader
	{
	public:
		VShader(ID3D11Device* device, std::string fileName, const D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT layoutNum);
		~VShader();

		void Apply(ID3D11DeviceContext* dContext)const override;
		void UnboundSRVAll(ID3D11DeviceContext* dContext)const override;

	private:
		ID3D11InputLayout* iLayout = nullptr;
		ID3D11VertexShader* vs = nullptr;
	};

	class DXLIB_DLL GShader : public Shader
	{
	public:
		GShader(ID3D11Device* device, std::string fileName = "");
		~GShader();

		void Apply(ID3D11DeviceContext* dContext)const override;
		void UnboundSRVAll(ID3D11DeviceContext* dContext)const override;
	private:
		ID3D11GeometryShader* gs = nullptr;
	};

	class DXLIB_DLL HShader : public Shader
	{
	public:
		HShader(ID3D11Device* device, std::string fileName = "");
		~HShader();

		void Apply(ID3D11DeviceContext* dContext)const override;
		void UnboundSRVAll(ID3D11DeviceContext* dContext)const override;
	private:
		ID3D11HullShader* hs = nullptr;
	};
	class DXLIB_DLL DShader : public Shader
	{
	public:
		DShader(ID3D11Device* device, std::string fileName = "");
		~DShader();

		void Apply(ID3D11DeviceContext* dContext)const override;
		void UnboundSRVAll(ID3D11DeviceContext* dContext)const override;
	private:
		ID3D11DomainShader* ds = nullptr;
	};


	class DXLIB_DLL PShader : public Shader
	{
	public:
		PShader(ID3D11Device* device, std::string fileName = "");
		~PShader();

		void Apply(ID3D11DeviceContext* dContext)const override;
		void UnboundSRVAll(ID3D11DeviceContext* dContext)const override;

	private:
		ID3D11PixelShader* ps = nullptr;
	};


	class DXLIB_DLL CShader : public Shader
	{
	public:
		CShader(ID3D11Device* device, const std::string CSfileName);
		~CShader();

		void Apply(ID3D11DeviceContext* dContext)const override;
		void UnboundSRVAll(ID3D11DeviceContext* dContext)const override;

	private:
		ID3D11ComputeShader* cs = nullptr;
	};
}
