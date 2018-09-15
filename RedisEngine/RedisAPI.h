#pragma once
#include "IRedisEngine.h"
#include "CommandHelper.h"

class CRedisAPI : public IRedisAPI
{
public:
	CRedisAPI();
	~CRedisAPI();

	// ִ�����ֱ��ִ�У�
	virtual BOOL RunCommand(int nRedisCmd, int nParamCount, ...);

	// ��ȡ�������
	virtual int GetResultCount();

	// ��ȡ���
	virtual BOOL GetResult(TRunCmdResult* pResult, int nCount);

	// ��ȡ��һ�β�����������
	virtual LPCSTR GetLastErrorDesc();

	// ��ȡ��һ��ִ�����
	virtual LPCSTR GetLastCmdStr();

	// ��ȡ��һ��ִ������ID
	virtual int GetLastCmdID();

	// �ܵ���ִ�ж�������
	virtual BOOL AddPipeCommand(int nRedisCmd, int nParamCount, ...);

	// ִ�йܵ�����
	virtual BOOL RunPipeCommand();

public:
	// ����
	BOOL Connect(const TRedisRunCfg& stRunCfg);

	// �Ͽ�����
	void Disconnect();

private:
	// Auth
	BOOL __Auth(LPCSTR szPassword);

	// PING 
	BOOL __Ping();

	//ִ������
	BOOL __RunCommand(LPCSTR szCmd);

	// ִ�ж�������
	BOOL __RunPipeCommand();

	// ������
	BOOL __HandleReply(redisReply* pReply);

	// ��¼����
	void __LogError(const char* szFormat, ...);

private:
	struct _TRunCmdResult
	{
		int nType;				/* EnRedisReply */
		__int64 n64Value;		/* The integer when type is enReply_Integer */
		std::string strResult;
	};

	struct _TRunCommand
	{
		// �������
		std::string strCommand;
		// ����ID
		int			nCommandID;
	};

	typedef		vector<_TRunCommand>		TVec_RunCommand;

private:
	// Context
	redisContext*			m_pContext;

	// ִ�н��
	vector<_TRunCmdResult>	m_vecResult;

	// ��������
	char					m_szErrDesc[2048];

	// ��������
	_TRunCommand			m_stRunCmd;

	// ��������
	TVec_RunCommand			m_vecCommand;

	// ����������
	CCommandHelper			m_oCommandData;
};

