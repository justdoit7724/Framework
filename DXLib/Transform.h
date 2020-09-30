#pragma once


namespace DX {
	class Transform
	{
	public:
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
		void SetRot(XMFLOAT3 _forward)
		{
			forward = _forward;
			XMFLOAT3 tempUP = (forward == UP) ? -FORWARD : UP;
			right = Cross(tempUP, forward);
			up = Cross(forward, right);
		}
		void SetRot(XMFLOAT3 _forward, XMFLOAT3 _up)
		{
			forward = _forward;
			up = _up;

			float l1 = Length(forward);
			float l2 = Length(up);
			if (l1 < 0.9f || l1 > 1.1f || l2 < 0.9f || l2 > 1.1f)
				int a = 0;
			right = Cross(_up, _forward);
		}
		void SetRot(XMFLOAT3 _forward, XMFLOAT3 _up, XMFLOAT3 _right) {

			forward = _forward;
			up = _up;
			right = _right;
		}
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

		XMFLOAT3 pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		XMFLOAT3 forward = FORWARD;
		XMFLOAT3 right = RIGHT;
		XMFLOAT3 up = UP;
		XMFLOAT3 scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	};
}

