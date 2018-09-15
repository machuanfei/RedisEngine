/********************************************************************
	created:	2017/11/17  17:57
	file base:	ThreadSuite
	file ext:	h
	author:		
	
	purpose:	�̼߳���
*********************************************************************/

#pragma once
#include "CriticalSession.h"
#include "LogicThread.h"



class CThreadSuite
{
private:
	typedef vector<CLogicThread*>		TThreadVec;
	typedef list<string>				TStringList;

private:
	//  ɾ��ȫ���߳�
	void __DeleteAllThread();

private:
	TThreadVec			m_vtThread;

	TRequestList		m_lsRequest;	// �����б�

	TResponseList		m_lsResponse;	//  �ظ��б�

	CCriticalSession	m_oThreadExchangeLock;   // ���߼��߳����ݽ���ʱ��Ҫ�����������

	TStringList			m_lsNotifyMsg;

public:
	CThreadSuite(void);
	~CThreadSuite(void);

	//   �������ô����߳�
	BOOL Create(const TRedisRunCfg  *pDBCfg);

	//   ��ȫ���̷߳���ֹͣ����
	void SendStopSingalToThread();

	//   ֹͣȫ���߳�
	BOOL StopAllThread(DWORD dwWaitTime);

	//   �������
	BOOL Safe_PushRequest(const TRedisRequest &stRequest);

	//   ɾ������
	BOOL Safe_CancelRequest(HRTRANSID hActID);

	//   �������� (�����̷߳��ʣ���Ҫ����)
	BOOL Safe_PopRequest(TRedisRequest &stRequest);

	//   ��ӻظ� (�����̷߳��ʣ���Ҫ����)
	BOOL Safe_PushResponse(const TRedisResponse& stResponse);

	//   ��ȡ�Ѿ������������. ���û��, �򷵻�NULL
	BOOL Safe_PopResponse(TResponseList &lsResponse);

	//   �����Ϣ (�����̷߳��ʣ���Ҫ����)
	BOOL Safe_PushNotify(const string& strMsg);
	
	//   ������Ϣ
	BOOL Safe_PopNotify(TStringList &lsMsg);	
};
