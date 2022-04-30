#pragma once
#include "Window.h"

class CtrlListBox :
    public WndCtrl
{
public:
    CtrlListBox(HINSTANCE hInstance, HWND parent, int x, int y, int w, int h, int key);
    ~CtrlListBox();
    
    void AddItem(std::string title);
    void DeleteItem(int index);

    void WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)override;

};

