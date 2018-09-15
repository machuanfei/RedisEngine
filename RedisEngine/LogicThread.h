/********************************************************************
	created:	2010/08/26
	created:	26:8:2010   10:58
	filename: 	f:\ws\All_CODE\SrcClient\BasicSys\Thread.h
	file path:	f:\ws\All_CODE\SrcClient\BasicSys
	file base:	Thread
	file ext:	h
	author:		Safish
	
	purpose:	启动线程去完成一系列工作
*********************************************************************/

#pragma once
#include "CriticalSession.h"

class CLogicThread;
class CThreadSuite;


//  向线程传递工作数据的现场
struct __ThreadWorkContext
{
	CLogicThread *	pThreadRoutine;
	TRedisRunCfg	stRunCfg;		// 运行配置
	CThreadSuite*	pThreadSuite;
} ;


class CLogicThread
{
private:
	typedef list<string>		TStringList;

private:
	
	HANDLE m_hThread;
	int m_nThreadID;	// 线程ID
	CCriticalSession  m_oThreadExchangeLock;   // 与逻辑线程数据交换时都要将这个锁锁上	
	__ThreadWorkContext m_oThreadContext;     // 线程现场 
	
	BOOL m_bStopRequest;		//是否有线程停止请求, 需要在另外线程中访问
	TStringList  m_lsNotifyString;  //通知描述
	TRedisRunCfg m_stRunCfg;	// 运行配置
	CThreadSuite*	m_pThreadSuite;		// 线程集

public:
	CLogicThread(void);
	~CLogicThread(void);

	BOOL Create(const TRedisRunCfg* pRunCfg, CThreadSuite* pSuite, int nThreadID);

	//  恢复线程运行 
	void  Resume();

	//  线程是否在工作
	BOOL  IsRunning();

	//    强制停止线程. 这个方法会先发送停止消息, 然后会等待线程结束. 当等待时间超过dwWaitTime(ms)后, 
	// 会使用terminate等API强制杀线程
	BOOL Close(DWORD dwWaitTime);

	//   发送一个停止命令. 让线程在合适的时候停止.
	void SendStopCommand();

	//   获取停止标记
	BOOL Safe_GetStopFlag();
};
