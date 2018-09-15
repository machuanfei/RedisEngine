// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�: 
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


// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
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