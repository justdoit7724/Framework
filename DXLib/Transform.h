#pragma once


namespace DX {
	class DXLIB_DLL Transform
	{
	public:
		Transform();

		XMMATRIX WorldMatrix()const;
		XMMATRIX S()const;
		XMMATRIX R()const;
		XMMATRIX T()const;

		XMFLOAT3 GetPos()const { return pos; }
		XMFLOAT3 GetForward()const {
			return forward;
		}
		XMFLOAT3 GetUp()const
		{
			return up;
		}
		XMFLOAT3 GetRight()const
		{
			return right;
		}
		XMFLOAT3 GetScale()const
		{
			return scale;
		}
		void SetTranslation(float x, float y, float z) {
			pos = { x,y,z };
		}
		void SetTranslation(XMFLOAT3 newPos) {
			pos = newPos;
		}
		void SetRot(XMFLOAT3 _forward);
		void SetRot(XMFLOAT3 _forward, XMFLOAT3 _up);
		void SetRot(XMFLOAT3 _forward, XMFLOAT3 _up, XMFLOAT3 _right);
		void Rotate(XMFLOAT3 axis, float rad);
		void SetScale(float x) {
			scale = { x,x,x };
		}
		void SetScale(float x, float y, float z) {
			scale = { x,y,z };
		}
		void SetScale(XMFLOAT3 newScale) {
			scale = newScale;
		}

	private:

		XMFLOAT3 pos;
		XMFLOAT3 forward;
		XMFLOAT3 right;
		XMFLOAT3 up;
		XMFLOAT3 scale;
	};
}

