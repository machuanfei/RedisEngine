#pragma once
#include "IRedisEngine.h"
#include "CommandHelper.h"

class CRedisAPI : public IRedisAPI
{
public:
	CRedisAPI();
	~CRedisAPI();

	// 执行命令（直接执行）
	virtual BOOL RunCommand(int nRedisCmd, int nParamCount, ...);

	// 获取结果数量
	virtual int GetResultCount();

	// 获取结果
	virtual BOOL GetResult(TRunCmdResult* pResult, int nCount);

	// 获取上一次操作错误描述
	virtual LPCSTR GetLastErrorDesc();

	// 获取上一次执行语句
	virtual LPCSTR GetLastCmdStr();

	// 获取上一次执行命令ID
	virtual int GetLastCmdID();

	// 管道：执行多条命令
	virtual BOOL AddPipeCommand(int nRedisCmd, int nParamCount, ...);

	// 执行管道命令
	virtual BOOL RunPipeCommand();

public:
	// 连接
	BOOL Connect(const TRedisRunCfg& stRunCfg);

	// 断开连接
	void Disconnect();

private:
	// Auth
	BOOL __Auth(LPCSTR szPassword);

	// PING 
	BOOL __Ping();

	//执行命令
	BOOL __RunCommand(LPCSTR szCmd);

	// 执行多条命令
	BOOL __RunPipeCommand();

	// 处理结果
	BOOL __HandleReply(redisReply* pReply);

	// 记录错误
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
		// 命令语句
		std::string strCommand;
		// 命令ID
		int			nCommandID;
	};

	typedef		vector<_TRunCommand>		TVec_RunCommand;

private:
	// Context
	redisContext*			m_pContext;

	// 执行结果
	vector<_TRunCmdResult>	m_vecResult;

	// 错误描述
	char					m_szErrDesc[2048];

	// 单条命令
	_TRunCommand			m_stRunCmd;

	// 多条命令
	TVec_RunCommand			m_vecCommand;

	// 命令数据类
	CCommandHelper			m_oCommandData;
};

