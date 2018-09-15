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
	//  ����һ���µ�ActionID
	HRTRANSID __AllocActionID();

	// ��¼����
	void __LogError(const char* szFormat, ...);

private:
	typedef map<int, CThreadSuite*>			TThreadSuiteMap;
	typedef map<HRTRANSID, TRedisRequest>	TRequestMap;
	typedef list<string>					TStringList;

private:
	char				m_szErrDesc[2048];	// ��������

	TThreadSuiteMap		m_oDBSuiteMap;		// �̼߳���

	TRequestMap			m_oRequestMap;		// �����б�, �û��ύ,δ�����ص����󶼻��������

	HRTRANSID			m_hMaxTransID;		// ��ǰ�����ȥ����actionid

	TStringList			m_lsRuntimeMsg;		// ����ʱ��Ϣ

	TRedisRunCfg		m_stRedisRunCfg;	// ��������
};

