// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "headers.h"
#include"dx9hook.h"
BOOL CheckProcess(const char* procressName)
{
	//获取进程名称
	//d:\Projects\Debug\Test.exe
	char szFileFullPath[MAX_PATH], szProcessName[MAX_PATH];
	GetModuleFileNameA(NULL, szFileFullPath, MAX_PATH);//获取文件路径
	int length = strlen(szFileFullPath); //获取长度
	for (int i = length - 1; i >= 0; i--)//从路径后面开始找\，即倒着找右斜杠
	{
		if (szFileFullPath[i] == '\\')//找到第一个\，就可以马上获取进程名称了
		{
			i++;
			for (int j = 0; i <= length; j++)//结束符\0不能少,即i=length
			{
				szProcessName[j] = szFileFullPath[i++];
			}
			break;
		}
	}

	//szProcessName就是本程序的进程名称了
	if (strcmp(szProcessName, procressName) != 0)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}
void initThread()
{
#ifdef _WIN64
	if (CheckProcess("crossfire_x64.exe")) //判断进程
	{
		Sleep(15000);
		if (!dx9_Initialize())
		{
			MessageBox(NULL, L"初始化失败", L"", MB_OK);
		}
	}
#else
	if (CheckProcess("crossfire.exe")) //判断进程
	{
		Sleep(15000);
		if (!dx9_Initialize())
		{
			MessageBox(NULL, L"初始化失败", L"", MB_OK);
		}
	}
#endif  
}
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		CloseHandle(CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(initThread), nullptr, 0, nullptr));
		break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

