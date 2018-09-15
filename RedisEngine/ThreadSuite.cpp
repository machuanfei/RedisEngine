#include "StdAfx.h"
#include "ThreadSuite.h"


CThreadSuite::CThreadSuite(void)
{
}


CThreadSuite::~CThreadSuite(void)
{
	__DeleteAllThread();
}


void CThreadSuite::__DeleteAllThread()
{
	for (int i = 0; i < (int)m_vtThread.size(); ++i)
	{
		CLogicThread *pLogicThread = m_vtThread[i];
		delete pLogicThread ;
	}
}


BOOL CThreadSuite::Create( const TRedisRunCfg *pRunCfg )
{
	if (pRunCfg == NULL || pRunCfg->dwThreadNum == 0)
	{
		return FALSE;
	}

	//  创建线程
	for (int i = 0; i < (int)pRunCfg->dwThreadNum; ++i)
	{
		CLogicThread *pLogicThread = new CLogicThread();
		if (!pLogicThread->Create(pRunCfg, this, i))
		{
			return FALSE;
		}
		pLogicThread->Resume();

		//  保存线程
		m_vtThread.push_back(pLogicThread);
	}


	return TRUE;
}


BOOL CThreadSuite::StopAllThread( DWORD dwWaitTime )
{
	BOOL bPeaceClose = TRUE;
	SendStopSingalToThread();
	
	//  关闭各线程
	for (int i = 0; i < (int)m_vtThread.size(); ++i)
	{ 
		CLogicThread *pLogicThread = m_vtThread[i];
		bPeaceClose &= pLogicThread->Close(dwWaitTime);
	}

	return bPeaceClose;
}


void CThreadSuite::SendStopSingalToThread()
{
	//  先向全部线程发送停止命令, 让各个线程准备停止
	for (int i = 0; i < (int)m_vtThread.size(); ++i)
	{
		CLogicThread *pLogicThread = m_vtThread[i];
		pLogicThread->SendStopCommand();
	}

}


BOOL CThreadSuite::Safe_PushRequest( const TRedisRequest &stRequest )
{
	m_oThreadExchangeLock.Lock();
	m_lsRequest.push_back(stRequest);
	m_oThreadExchangeLock.Unlock();
	return TRUE;
}


BOOL CThreadSuite::Safe_CancelRequest(HRTRANSID hActID )
{
	BOOL bRet = FALSE;
	m_oThreadExchangeLock.Lock();
	
	// 查找
	TRequestList::iterator itRequest = m_lsRequest.begin();
	for ( ; itRequest != m_lsRequest.end(); ++itRequest)
	{
		if (itRequest->hActID == hActID)
		{
			m_lsRequest.erase(itRequest);
			bRet = TRUE;
			break;
		}
	}

	m_oThreadExchangeLock.Unlock();

	return bRet;
}


BOOL CThreadSuite::Safe_PopRequest(TRedisRequest &stRequest)
{
	BOOL bRet = FALSE;
	m_oThreadExchangeLock.Lock();
	
	if (!m_lsRequest.empty())
	{
		stRequest = m_lsRequest.front();
		m_lsRequest.pop_front();
		bRet = TRUE;
	}

	m_oThreadExchangeLock.Unlock();
	return bRet;
}


BOOL CThreadSuite::Safe_PushResponse(const TRedisResponse& stResponse)
{
	m_oThreadExchangeLock.Lock();

	m_lsResponse.push_back(stResponse);

	m_oThreadExchangeLock.Unlock();
	return TRUE;
}


BOOL CThreadSuite::Safe_PopResponse(TResponseList &lsResponse)
{
	BOOL bRet = FALSE;
	m_oThreadExchangeLock.Lock();

	if (!m_lsResponse.empty())
	{
		// 交换
		m_lsResponse.swap(lsResponse);
		bRet = TRUE;
	}

	m_oThreadExchangeLock.Unlock();
	return bRet;
}


BOOL CThreadSuite::Safe_PushNotify(const string& strMsg)
{
	m_oThreadExchangeLock.Lock();
	m_lsNotifyMsg.push_back(strMsg);
	m_oThreadExchangeLock.Unlock();

	return TRUE;
}


BOOL CThreadSuite::Safe_PopNotify(TStringList &lsMsg)
{
	BOOL bRet = FALSE;
	m_oThreadExchangeLock.Lock();

	if (!m_lsNotifyMsg.empty())
	{
		// 交换
		m_lsNotifyMsg.swap(lsMsg);
		bRet = TRUE;
	}

	m_oThreadExchangeLock.Unlock();
	return bRet;
}

