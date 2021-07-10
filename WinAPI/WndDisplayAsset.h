#pragma once
#include "WndDisplay.h"
class WndDisplayAsset :
    public WndDisplay
{
public:
    WndDisplayAsset(HINSTANCE hInstance, HWND parent, int x, int y, int width, int height, int msaa);

};

