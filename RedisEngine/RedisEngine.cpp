#include "stdafx.h"
#include "RedisEngine.h"


CRedisEngine::CRedisEngine() : m_hMaxTransID(INVALID_HRTRANSID)
{
	m_szErrDesc[0] = 0;
}


CRedisEngine::~CRedisEngine()
{
	StopService(0);
}


BOOL CRedisEngine::Create(TRedisRunCfg* pRunCfg)
{
	m_stRedisRunCfg = *pRunCfg;
	return TRUE;
}


void CRedisEngine::Release()
{
	delete this;
}


BOOL CRedisEngine::StartService()
{
	if (!m_oDBSuiteMap.empty())
	{
		__LogError("%s: Redis Engine is already RUNNING!", __FUNCTION__);
		return FALSE;
	}

	//  按配置启动全部的线程
	for (int i = 0; i < m_stRedisRunCfg.dwSuiteNum; ++i)
	{
		CThreadSuite *pThreadSuite = new CThreadSuite();
		m_oDBSuiteMap[i] = pThreadSuite;
		if (!pThreadSuite->Create(&m_stRedisRunCfg))
		{
			StopService(0);
			__LogError("%s: Thread suite can not start!  nSuiteID=%d", __FUNCTION__, i);
			return FALSE;
		}
	}

	return TRUE;
}


BOOL CRedisEngine::StopService(DWORD dwWaitTime)
{
	//  先发命令让线程组停止
	TThreadSuiteMap::iterator itThreadSuite = m_oDBSuiteMap.begin();
	for (; itThreadSuite != m_oDBSuiteMap.end(); ++itThreadSuite)
	{
		CThreadSuite *pSuite = (*itThreadSuite).second;
		pSuite->SendStopSingalToThread();
	}

	BOOL bPeaceClose = TRUE;

	//  等待每个线程组停止
	DWORD dwStartTick = ::GetTickCount();
	for (itThreadSuite = m_oDBSuiteMap.begin(); itThreadSuite != m_oDBSuiteMap.end(); ++itThreadSuite)
	{
		CThreadSuite *pSuite = (*itThreadSuite).second;
		DWORD dwNowTick = ::GetTickCount();
		if (dwNowTick > dwStartTick + dwWaitTime)
		{
			bPeaceClose &= pSuite->StopAllThread(0);
		}
		else
		{
			bPeaceClose &= pSuite->StopAllThread(dwWaitTime - (dwNowTick - dwStartTick));
		}

		//  释放suite
		delete pSuite;
	}

	m_oDBSuiteMap.clear();
	return bPeaceClose;
}


BOOL CRedisEngine::CancelRequest(HRTRANSID hAID)
{
	TRequestMap::iterator itRequest = m_oRequestMap.find(hAID);
	if (itRequest == m_oRequestMap.end())
	{
		__LogError("Task not exist! hAID=%ld", hAID);
		return TRUE;
	}

	// 查找出对应的线程集
	TRedisRequest &stRequest = (*itRequest).second;
	TThreadSuiteMap::iterator itSuite = m_oDBSuiteMap.find(stRequest.nSuiteID);
	if (itSuite == m_oDBSuiteMap.end())
	{
		__LogError("Thread suite not exist! hAID=%ld  nSuiteID=%d", hAID, stRequest.nSuiteID);
		return FALSE;
	}
	CThreadSuite *pThreadSuite = (*itSuite).second;
	if (!pThreadSuite->Safe_CancelRequest(stRequest.hActID))
	{
		__LogError("Task is running, can not CANCEL!  hActID=%ld", hAID);
		return FALSE;
	}
	else
	{
		//  从请求列表中删除
		m_oRequestMap.erase(itRequest);
	}

	__LogError("CANCEL  hActID=%ld OK", hAID);
	return TRUE;
}


LPCSTR CRedisEngine::GetLastErrorMsg()
{
	return m_szErrDesc;
}


RedisEngine::HRTRANSID CRedisEngine::Request(IRedisTask* pTask, IRedisTaskRetSink* pRetSink, int nSuiteID)
{
	if (pTask == NULL)
	{
		return INVALID_HRTRANSID;
	}

	TThreadSuiteMap::iterator itSuite = m_oDBSuiteMap.find(nSuiteID);
	if (itSuite == m_oDBSuiteMap.end())
	{
		__LogError("Thread suite not exist! nSuiteID=%d", nSuiteID);
		return INVALID_HRTRANSID;
	}

	CThreadSuite* pThreadSuite = (*itSuite).second;

	// 分配一个新的ID, 这里假设都能分配成功。因为42亿个数字全被占用的可能性应该没有
	HRTRANSID hActID = __AllocActionID();
	TRedisRequest& stRequest = m_oRequestMap[hActID];
	stRequest.hActID = hActID;
	stRequest.nSuiteID = nSuiteID;
	stRequest.pTask = pTask;
	stRequest.pRetSink = pRetSink;

	if (!pThreadSuite->Safe_PushRequest(stRequest))
	{
		__LogError("%s: Safe_AddRequest return false! ", __FUNCTION__);
		m_oRequestMap.erase(hActID);
		return INVALID_HRTRANSID;
	}

	return hActID;
}


BOOL CRedisEngine::PopFinishedResult()
{
	BOOL bRet = FALSE;
	TResponseList lsResponse;
	TThreadSuiteMap::iterator itThreadSuite = m_oDBSuiteMap.begin();
	for (; itThreadSuite != m_oDBSuiteMap.end(); ++itThreadSuite)
	{
		CThreadSuite *pSuite = (*itThreadSuite).second;
		if (pSuite->Safe_PopResponse(lsResponse))
		{
			bRet = TRUE;

			TResponseList::iterator itResponse = lsResponse.begin();
			for (; itResponse != lsResponse.end(); ++itResponse)
			{
				const TRedisResponse& stResponse = *itResponse;

				// 根据回复中事务ID查找回调
				TRequestMap::iterator itRequest = m_oRequestMap.find(stResponse.hActID);
				if (itRequest != m_oRequestMap.end())
				{
					IRedisTaskRetSink* pRetSink = itRequest->second.pRetSink;
					if (pRetSink != NULL)
					{
						pRetSink->OnRedisTaskRet(stResponse.hActID, &stResponse);
					}

					// 移除
					m_oRequestMap.erase(itRequest);
				}
			}
		}
	}

	return bRet;
}


BOOL CRedisEngine::PopRuntimeMsg(LPSTR pBuffer, DWORD dwBufferLen)
{
	//  如果消息队列为空, 先到ThreadSuite取一次
	if (m_lsRuntimeMsg.empty())
	{
		TThreadSuiteMap::iterator itThreadSuite = m_oDBSuiteMap.begin();
		for (; itThreadSuite != m_oDBSuiteMap.end(); ++itThreadSuite)
		{
			CThreadSuite *pSuite = (*itThreadSuite).second;
			pSuite->Safe_PopNotify(m_lsRuntimeMsg);

			if (!m_lsRuntimeMsg.empty())
			{
				break;
			}
		}
	}

	if (m_lsRuntimeMsg.empty())
	{
		return FALSE;
	}

	if (dwBufferLen > 0)
	{
		//lstrcpyn(pBuffer, m_lsRuntimeMsg.front().c_str(), dwBufferLen);
		strncpy_s(pBuffer, dwBufferLen, m_lsRuntimeMsg.front().c_str(), dwBufferLen - 1);
		pBuffer[dwBufferLen - 1] = '\0';
	}

	m_lsRuntimeMsg.pop_front();
	return TRUE;
}


HRTRANSID CRedisEngine::__AllocActionID()
{
	++m_hMaxTransID;
	while (m_hMaxTransID == INVALID_HRTRANSID || m_oRequestMap.find(m_hMaxTransID) != m_oRequestMap.end())
	{
		++m_hMaxTransID;
	}

	return m_hMaxTransID;
}


void CRedisEngine::__LogError(const char* szFormat, ...)
{
	va_list param;
	va_start(param, szFormat);
	vsnprintf_s(m_szErrDesc, sizeof(m_szErrDesc), _TRUNCATE, szFormat, param);
	va_end(param);
}
