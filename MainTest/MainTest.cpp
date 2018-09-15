// MainTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <assert.h>
#include "TestTask.h"


// 加载模块
IRedisEngine* __CreateRedisEngine(CInterfaceLoader& oLoader)
{
#ifdef _DEBUG
#define REDIS_ENGINE_DLL "./RedisEngine_d.dll"
#else
#define REDIS_ENGINE_DLL "./RedisEngine.dll"
#endif


	IRedisEngine* pEngine = (IRedisEngine*)oLoader.CreateInterface(REDIS_ENGINE_DLL, "");
	if (pEngine == NULL)
	{
		cout << __FUNCTION__ << ": Load RedisEngine.dll FAIL!" << endl;
		return NULL;
	}

	// 创建
	TRedisRunCfg stRunCfg;
	//lstrcpyn(stRunCfg.szIP, "127.0.0.1", sizeof(stRunCfg.szIP));
	sprintf_s(stRunCfg.szIP, "127.0.0.1");
	stRunCfg.dwPort = 6379;
	stRunCfg.dwThreadNum = 1;
	stRunCfg.dwSuiteNum = 1;
	stRunCfg.bHasPassword = true;
	//lstrcpyn(stRunCfg.szPassword, "123457", sizeof(stRunCfg.szPassword));
	sprintf_s(stRunCfg.szPassword, "123457");
	if (!pEngine->Create(&stRunCfg))
	{
		cout << __FUNCTION__ << ":  Create RedisEngine FAIL!" << endl;
		return NULL;
	}

	return pEngine;
}


int main()
{
	CInterfaceLoader oLoader;
	IRedisEngine* pRedisEngine = __CreateRedisEngine(oLoader);
	assert(pRedisEngine != NULL);

	// 启动
	assert(pRedisEngine->StartService() != FALSE);

	// 添加任务
	for (int nIndex = 0; nIndex < 1; ++nIndex)
	{
		CTestTask oTestTask;
		oTestTask.Set(nIndex * 5);
		pRedisEngine->Request(&oTestTask, NULL, nIndex);

		LPCSTR pResult = oTestTask.GetRunResult();


	}

	Sleep(100000);
	pRedisEngine->StopService(4000);
	system("pause");
    return 0;
}

