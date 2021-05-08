#pragma once

#define NOMINMAX
#include <Windows.h>
#include <string>
#include <set>
#include <map>
#include <queue>
#include <memory>
#include <algorithm>
#include <vector>
#include <time.h>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <mutex>
#include <cstdarg>

#include <unordered_map>
#include <unordered_set>
#include <stack>

#include "..\DXLib\DXLibExport.h"

#ifdef _WIN64

#ifdef _DEBUG
#pragma comment(lib, __FILE__"\\..\\..\\x64\\Debug\\DXLib.lib")
#else
#pragma comment(lib, __FILE__"\\..\\..\\x64\\Release\\DXLib.lib")
#endif

#else

#ifdef _DEBUG
#pragma comment(lib, __FILE__"\\..\\..\\Debug\\DXLib.lib")
#else
#pragma comment(lib, __FILE__"\\..\\..\\Release\\DXLib.lib")
#endif

#endif

#define SAFEDELETE(p)\
{\
if (p) delete (p); \
(p)=nullptr;\
}

#define SAFEDELETEARR(p)\
{\
if (p) delete[] (p); \
(p)=nullptr;\
}

#define ASSERT_MSG(b,str)\
{\
if(!(b))\
{\
std::string msg;\
msg += __FILE__;\
msg += "(";\
msg += __LINE__;\
msg += ")\n";\
msg += str;\
MessageBoxA(nullptr, msg.c_str(), "error", MB_OK);\
}\
}


#include "Math.h"



