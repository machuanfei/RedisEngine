#pragma once
#include "ThreadSuite.h"


class CRedisEngine : public IRedisEngine
{
public:
	CRedisEngine();
	~CRedisEngine();


	virtual BOOL Create(TRedisRunCfg* pRunCfg);

	virtual void Release();

	virtual BOOL StartService();

	virtual BOOL StopService(DWORD dwWaitTime);

	virtual BOOL CancelRequest(HRTRANSID hAID);

	virtual LPCSTR GetLastErrorMsg();

	virtual HRTRANSID Request(IRedisTask* pTask, IRedisTaskRetSink* pRetSink, int nSuiteID);

	virtual BOOL PopFinishedResult();

	virtual BOOL PopRuntimeMsg(LPSTR pBuffer, DWORD dwBufferLen);

private:
	//  分配一个新的ActionID
	HRTRANSID __AllocActionID();

	// 记录错误
	void __LogError(const char* szFormat, ...);

private:
	typedef map<int, CThreadSuite*>			TThreadSuiteMap;
	typedef map<HRTRANSID, TRedisRequest>	TRequestMap;
	typedef list<string>					TStringList;

private:
	char				m_szErrDesc[2048];	// 错误描述

	TThreadSuiteMap		m_oDBSuiteMap;		// 线程集合

	TRequestMap			m_oRequestMap;		// 请求列表, 用户提交,未被返回的请求都会放在这里

	HRTRANSID			m_hMaxTransID;		// 当前分配出去最大的actionid

	TStringList			m_lsRuntimeMsg;		// 运行时信息

	TRedisRunCfg		m_stRedisRunCfg;	// 运行配置
};

