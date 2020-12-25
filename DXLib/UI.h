#pragma once

#include "ObserverDP.h"
#include "Object.h"


namespace DX {

	class Transform;
	class Camera;
	class UICanvas;

	class DXLIB_DLL UI : public Object
	{
	public:
		UI(ID3D11Device* device, ID3D11DeviceContext* dContext, XMFLOAT2 pivot, XMFLOAT2 size, float zDepth, ID3D11ShaderResourceView* srv);
		~UI();

		virtual void Update(ID3D11DeviceContext* dContext, int iScnWidth, int iScnHeight, XMFLOAT2 mousePos, UICanvas* canvas);
		void SetSRV(ID3D11ShaderResourceView* srv);
		void SetMulColor(ID3D11DeviceContext* dContext, XMFLOAT3 c);

	protected:

		ID3D11ShaderResourceView* srv;

		XMFLOAT4 mulColor;
		XMFLOAT2 size;
	};

	class DXLIB_DLL UIButton : public UI, public Subject
	{
	public:
		UIButton(ID3D11Device* device, ID3D11DeviceContext* dContext, XMFLOAT2 pivot, XMFLOAT2 size, ID3D11ShaderResourceView* idleSRV);

		void Update(ID3D11DeviceContext* dContext, int iScnWidth, int iScnHeight, XMFLOAT2 mousePos, UICanvas* canvas) override;

		void SetNotify(int id, const void* data) { notifyID = id; notifyData = data; }

	private:

		int notifyID = NOTIFY_NONE;
		const void* notifyData = nullptr;

		Geometrics::Plane bound;
	};


	class DXLIB_DLL UICanvas
	{
	public:
		UICanvas(ID3D11DeviceContext* dContext, int iScnWidth, int iScnHeight);
		~UICanvas();

		const float totalWidth, totalHeight;

		const Camera* GetCamera() { return camera; }

		void Add(UI* ui);
		void Remove(UI* ui);
		void Update(XMFLOAT2 mousePos);

	private:
		Camera* camera;
		std::vector<UI*> m_vUI;

		ID3D11DeviceContext* m_dxDContext;
	};
}

