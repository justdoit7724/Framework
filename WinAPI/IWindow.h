#pragma once


class IWindow
{
public:
	virtual void WndProc(unsigned int msg, unsigned long long wparam, long long lparam) = 0;

};