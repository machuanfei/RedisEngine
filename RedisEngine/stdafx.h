// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件: 
#include <WinSock2.h>
//#pragma comment(lib, "ws2_32.lib")
#include <windows.h>
#include <tchar.h>
#include <string>
#include <iostream>
#include <vector>
#include <list>
#include <map>
using namespace std;


// TODO: 在此处引用程序需要的其他头文件
#include "hiredis/hiredis.h"
#include "IRedisEngine.h"
using namespace RedisEngine;

#ifdef _DEBUG
#pragma comment(lib, "Win32_Interop_d.lib")
#pragma comment(lib, "hiredis_d.lib")
#else
#pragma comment(lib, "Win32_Interop.lib")
#pragma comment(lib, "hiredis.lib")
#endif


typedef		list<TRedisRequest>				TRequestList;
typedef		list<TRedisResponse>			TResponseList;