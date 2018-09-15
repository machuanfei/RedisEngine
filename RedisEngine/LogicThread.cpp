#include "stdafx.h"
#include "LogicThread.h"
#include "RedisAPI.h"
#include "ThreadSuite.h"


DWORD WINAPI CThread_WorkProc(LPVOID lpParameter)
{
	
	__ThreadWorkContext *pThreadContext =  reinterpret_cast<__ThreadWorkContext *>(lpParameter);
	CLogicThread *pThreadRoutine = pThreadContext->pThreadRoutine;
	CThreadSuite* pThreadSuite = pThreadContext->pThreadSuite;
	
	const TRedisRunCfg& stRunCfg = pThreadContext->stRunCfg;
	TRedisRequest stRequest;

	while(!pThreadRoutine->Safe_GetStopFlag())
	{
		//  如果没请求, 休息一下
		if (!pThreadSuite->Safe_PopRequest(stRequest))
		{
			::Sleep(16);
			continue;
		}
		
		//  断线不断重连, 检查停止标记
		CRedisAPI  oRedisAPI;
		while (!oRedisAPI.Connect(stRunCfg))
		{
			//  构造消息, 通知外部线程
			/*char szErrorMsg[2048];
			_stprintf_s(szErrorMsg, sizeof(szErrorMsg), "Connect redis server FAIL!  IP:%s  Port:%d  errorDesc:%s", stRunCfg.szIP, stRunCfg.dwPort, oRedisAPI.GetLastErrorDesc());
			pThreadSuite->Safe_PushNotify(szErrorMsg);*/

			if (pThreadRoutine->Safe_GetStopFlag())
			{
				return 0;
			}
			::Sleep(1000);  //连不上也先停1秒
		}

		// 执行操作
		stRequest.pTask->Do(&oRedisAPI);
		// 添加执行结果
		pThreadSuite->Safe_PushNotify(stRequest.pTask->GetRunResult());

		// 添加已处理请求
		TRedisResponse stResponse = { stRequest.hActID, stRequest.pTask->GetTaskType(), "", stRequest.pTask->GetContextLen() };
		stRequest.pTask->ExportResult(stResponse.arrContext, stResponse.nContextLen);
		pThreadSuite->Safe_PushResponse(stResponse);
	}
	
	return 0;
}


CLogicThread::CLogicThread(void) : m_hThread(0), m_nThreadID(0), m_pThreadSuite(NULL)
{
}


CLogicThread::~CLogicThread(void)
{
	
}


BOOL CLogicThread::Create(const TRedisRunCfg* pRunCfg, CThreadSuite* pSuite, int nThreadID)
{
	m_stRunCfg = *pRunCfg;
	m_nThreadID = nThreadID;
	m_pThreadSuite = pSuite;
	return TRUE;
}


void CLogicThread::Resume()
{
	if (IsRunning())
	{
		return;
	}

	m_bStopRequest = FALSE;

	DWORD dwThreadID = 0;
	m_oThreadContext.pThreadRoutine = this;
	m_oThreadContext.stRunCfg = m_stRunCfg;
	m_oThreadContext.pThreadSuite = m_pThreadSuite;
	
	// 创建线程
	m_hThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CThread_WorkProc, (LPVOID)&m_oThreadContext, 0, &dwThreadID);
	if (m_hThread)
	{		
		::ResumeThread(m_hThread);
	}
}


BOOL CLogicThread::IsRunning()
{
	if (m_hThread == 0)
	{
		return FALSE;
	}

	BOOL bRunning = (::WaitForSingleObject(m_hThread, 0) == WAIT_TIMEOUT);
	return bRunning;
}


void CLogicThread::SendStopCommand()
{
	m_oThreadExchangeLock.Lock();
	m_bStopRequest = TRUE;
	m_oThreadExchangeLock.Unlock();	
}


BOOL CLogicThread::Close( DWORD dwWaitTime )
{
	BOOL bPeaceClose = TRUE;  //和平关闭, 没发生过强制杀线程的事件

	//  先发送线程停止消息
	SendStopCommand();

	//  等待指定的时间
	const int WAIT_ELAPSE = 16;//  每次等16ms
	dwWaitTime = dwWaitTime ? dwWaitTime :WAIT_ELAPSE;  //至少等16ms, 让线程有机会释放
	DWORD dwElapse = 0;
	BOOL bRunning = IsRunning();
	while(dwElapse < dwWaitTime)
	{
		if (!bRunning)
		{
			break;
		}

		dwElapse += WAIT_ELAPSE;
		::Sleep(WAIT_ELAPSE);
		bRunning = IsRunning();
	}

	//  如果线程还在跑, 就强行杀掉
	if (bRunning)
	{		
		::TerminateThread(m_hThread, -1);
		bPeaceClose = FALSE;   //杀了线程, 不和平了
	}

	//  关闭线程句柄
	if (m_hThread != 0)
	{
		::CloseHandle(m_hThread);
		m_hThread = 0;
	}

	return bPeaceClose;
}


BOOL CLogicThread::Safe_GetStopFlag()
{
	BOOL bStop;
	m_oThreadExchangeLock.Lock();
	bStop = m_bStopRequest;
	m_oThreadExchangeLock.Unlock();	
	return bStop;
}
