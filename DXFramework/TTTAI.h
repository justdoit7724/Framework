#pragma once

class Tictactoe;
class TTTRender;

class TTTAI{
public:
	TTTAI(Tictactoe*const* ppRule, TTTRender* const* ppRender);

	void Move();

private:
	Tictactoe*const* ppRule;
	TTTRender* const* ppRender;
};
