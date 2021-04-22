#pragma once
#include "WndDisplay.h"
class WndDisplayModel :
    public WndDisplay
{
public:
    WndDisplayModel(HINSTANCE hInstance, HWND parent, int x, int y, int width, int height, int msaa);

};

