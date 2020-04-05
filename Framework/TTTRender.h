#pragma once

class UIButton;
class UICanvas;
class Observer;
enum SpaceKind;

class TTTRender
{
public:
	TTTRender();

	void Update();
	void MoveRender(int x, int y, SpaceKind kind);
	void SetClickFunc(Observer* obs);

private:

	UICanvas* canvas;
	UIButton* board[3][3];
	ID3D11ShaderResourceView* oMoveSRV;
	ID3D11ShaderResourceView* xMoveSRV;
	ID3D11ShaderResourceView* noneMoveSRV;

};
