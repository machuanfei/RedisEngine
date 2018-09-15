/********************************************************************
	created:	2010/08/26
	created:	26:8:2010   10:58
	filename: 	f:\ws\All_CODE\SrcClient\BasicSys\Thread.h
	file path:	f:\ws\All_CODE\SrcClient\BasicSys
	file base:	Thread
	file ext:	h
	author:		Safish
	
	purpose:	�����߳�ȥ���һϵ�й���
*********************************************************************/

#pragma once
#include "CriticalSession.h"

class CLogicThread;
class CThreadSuite;


//  ���̴߳��ݹ������ݵ��ֳ�
struct __ThreadWorkContext
{
	CLogicThread *	pThreadRoutine;
	TRedisRunCfg	stRunCfg;		// ��������
	CThreadSuite*	pThreadSuite;
} ;


class CLogicThread
{
private:
	typedef list<string>		TStringList;

private:
	
	HANDLE m_hThread;
	int m_nThreadID;	// �߳�ID
	CCriticalSession  m_oThreadExchangeLock;   // ���߼��߳����ݽ���ʱ��Ҫ�����������	
	__ThreadWorkContext m_oThreadContext;     // �߳��ֳ� 
	
	BOOL m_bStopRequest;		//�Ƿ����߳�ֹͣ����, ��Ҫ�������߳��з���
	TStringList  m_lsNotifyString;  //֪ͨ����
	TRedisRunCfg m_stRunCfg;	// ��������
	CThreadSuite*	m_pThreadSuite;		// �̼߳�

public:
	CLogicThread(void);
	~CLogicThread(void);

	BOOL Create(const TRedisRunCfg* pRunCfg, CThreadSuite* pSuite, int nThreadID);

	//  �ָ��߳����� 
	void  Resume();

	//  �߳��Ƿ��ڹ���
	BOOL  IsRunning();

	//    ǿ��ֹͣ�߳�. ����������ȷ���ֹͣ��Ϣ, Ȼ���ȴ��߳̽���. ���ȴ�ʱ�䳬��dwWaitTime(ms)��, 
	// ��ʹ��terminate��APIǿ��ɱ�߳�
	BOOL Close(DWORD dwWaitTime);

	//   ����һ��ֹͣ����. ���߳��ں��ʵ�ʱ��ֹͣ.
	void SendStopCommand();

	//   ��ȡֹͣ���
	BOOL Safe_GetStopFlag();
};
