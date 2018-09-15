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
		//  ���û����, ��Ϣһ��
		if (!pThreadSuite->Safe_PopRequest(stRequest))
		{
			::Sleep(16);
			continue;
		}
		
		//  ���߲�������, ���ֹͣ���
		CRedisAPI  oRedisAPI;
		while (!oRedisAPI.Connect(stRunCfg))
		{
			//  ������Ϣ, ֪ͨ�ⲿ�߳�
			/*char szErrorMsg[2048];
			_stprintf_s(szErrorMsg, sizeof(szErrorMsg), "Connect redis server FAIL!  IP:%s  Port:%d  errorDesc:%s", stRunCfg.szIP, stRunCfg.dwPort, oRedisAPI.GetLastErrorDesc());
			pThreadSuite->Safe_PushNotify(szErrorMsg);*/

			if (pThreadRoutine->Safe_GetStopFlag())
			{
				return 0;
			}
			::Sleep(1000);  //������Ҳ��ͣ1��
		}

		// ִ�в���
		stRequest.pTask->Do(&oRedisAPI);
		// ���ִ�н��
		pThreadSuite->Safe_PushNotify(stRequest.pTask->GetRunResult());

		// ����Ѵ�������
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
	
	// �����߳�
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
	BOOL bPeaceClose = TRUE;  //��ƽ�ر�, û������ǿ��ɱ�̵߳��¼�

	//  �ȷ����߳�ֹͣ��Ϣ
	SendStopCommand();

	//  �ȴ�ָ����ʱ��
	const int WAIT_ELAPSE = 16;//  ÿ�ε�16ms
	dwWaitTime = dwWaitTime ? dwWaitTime :WAIT_ELAPSE;  //���ٵ�16ms, ���߳��л����ͷ�
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

	//  ����̻߳�����, ��ǿ��ɱ��
	if (bRunning)
	{		
		::TerminateThread(m_hThread, -1);
		bPeaceClose = FALSE;   //ɱ���߳�, ����ƽ��
	}

	//  �ر��߳̾��
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
