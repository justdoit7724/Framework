#pragma once
#include "IWindow.h"
class WndTest :
	public IWindow
{
public:


	// Inherited via IWindow
	virtual void WndProc(unsigned int msg, unsigned long long wparam, long long lparam) override;

private:

};

