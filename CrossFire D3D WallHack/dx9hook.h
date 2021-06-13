#pragma once
#include<Windows.h>
#include "d3d9.h"
#include "tchar.h"
#include"./minhook/MinHook.h"
#if defined _M_X64
#pragma comment(lib,"./minhook/libMinHook.x64.lib") 
#elif defined _M_IX86
#pragma comment(lib,"./minhook/libMinHook.x86.lib") 
#endif
#pragma comment(lib,"d3d9.lib") 
bool dx9_Initialize();