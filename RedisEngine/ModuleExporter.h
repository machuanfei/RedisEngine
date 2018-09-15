#pragma once


#if(defined(_WIN32) || defined(_WIN64))
extern "C" __declspec(dllexport) void * GetModuleInterface(LPCSTR pszInterfaceName);
#else
extern "C" void * GetModuleInterface(LPCSTR pszInterfaceName);
#endif









