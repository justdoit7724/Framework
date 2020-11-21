#pragma once

#define NOMINMAX
#include <Windows.h>
#include <string>
#include <set>
#include <vector>
#include <time.h>
#include <assert.h>

#include <unordered_map>
#include <unordered_set>

#include <DXLibExport.h>
#pragma comment(lib,"DXLib.lib")

#define SAFEDELETE(p)\
if (p) delete p; \
p=nullptr;

#define SAFEDELETEARR(p)\
if (p) delete[] p; \
p=nullptr;

extern DX::Graphic* g_dxGraphic;


