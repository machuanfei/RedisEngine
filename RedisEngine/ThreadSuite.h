/********************************************************************
	created:	2017/11/17  17:57
	file base:	ThreadSuite
	file ext:	h
	author:		
	
	purpose:	线程集合
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
	//  删除全部线程
	void __DeleteAllThread();

private:
	TThreadVec			m_vtThread;

	TRequestList		m_lsRequest;	// 请求列表

	TResponseList		m_lsResponse;	//  回复列表

	CCriticalSession	m_oThreadExchangeLock;   // 与逻辑线程数据交换时都要将这个锁锁上

	TStringList			m_lsNotifyMsg;

public:
	CThreadSuite(void);
	~CThreadSuite(void);

	//   根据配置创建线程
	BOOL Create(const TRedisRunCfg  *pDBCfg);

	//   向全部线程发送停止命令
	void SendStopSingalToThread();

	//   停止全部线程
	BOOL StopAllThread(DWORD dwWaitTime);

	//   添加请求
	BOOL Safe_PushRequest(const TRedisRequest &stRequest);

	//   删除请求
	BOOL Safe_CancelRequest(HRTRANSID hActID);

	//   弹出请求 (由子线程访问，需要加锁)
	BOOL Safe_PopRequest(TRedisRequest &stRequest);

	//   添加回复 (由子线程访问，需要加锁)
	BOOL Safe_PushResponse(const TRedisResponse& stResponse);

	//   获取已经处理过的请求. 如果没有, 则返回NULL
	BOOL Safe_PopResponse(TResponseList &lsResponse);

	//   添加消息 (由子线程访问，需要加锁)
	BOOL Safe_PushNotify(const string& strMsg);
	
	//   弹出消息
	BOOL Safe_PopNotify(TStringList &lsMsg);	
};
