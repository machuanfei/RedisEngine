#include "stdafx.h"
#include "RedisAPI.h"


CRedisAPI::CRedisAPI() : m_pContext(NULL)
{
}


CRedisAPI::~CRedisAPI()
{
	Disconnect();
}


BOOL CRedisAPI::RunCommand(int nRedisCmd, int nParamCount, ...)
{
	// 记录ID
	m_stRunCmd.nCommandID = nRedisCmd;
	m_stRunCmd.strCommand.clear();

	// 解析参数
	vector<std::string>	vecParam;
	va_list param;
	va_start(param, nParamCount);
	for (int nIndex = 0; nIndex < nParamCount; ++nIndex)
	{
		vecParam.push_back(va_arg(param, char*));
	}
	va_end(param);

	// 构造命令字符串
	if (!m_oCommandData.FormatCmd(nRedisCmd, &vecParam[0], nParamCount, m_stRunCmd.strCommand))
	{
		__LogError("%s: %s", __FUNCTION__, m_oCommandData.GetLastErrDesc());
		return FALSE;
	}

	return __RunCommand(m_stRunCmd.strCommand.c_str());
}


int CRedisAPI::GetResultCount()
{
	return m_vecResult.size();
}


BOOL CRedisAPI::GetResult(TRunCmdResult* pResult, int nCount)
{
	for ( int nIndex = 0; nIndex < m_vecResult.size() && nIndex < nCount; ++nIndex)
	{
		const _TRunCmdResult& stSrcResult = m_vecResult[nIndex];
		TRunCmdResult& stDestResult = pResult[nIndex];
		if (stDestResult.type != stSrcResult.nType)
		{
			__LogError("%s: Result type not match!  nDestType=%d  nType=%d nIndex=%d", __FUNCTION__, stDestResult.type, stSrcResult.nType, nIndex);
			return FALSE;
		}

		// 处理结果
		switch (stSrcResult.nType)
		{
		case enReply_Error:
		case enReply_String:
		case enReply_Status:
		{
			//lstrcpyn(stDestResult.str, stSrcResult.strResult.c_str(), stDestResult.len);
			strncpy_s(stDestResult.str, stDestResult.len, stSrcResult.strResult.c_str(), stDestResult.len - 1);
			stDestResult.str[stDestResult.len - 1] = '\0';
		}
		break;

		case enReply_Integer:
		{
			stDestResult.integer = stSrcResult.n64Value;
		}
		break;

		default:
			__LogError("%s: ResultType=%d is not processed!", stSrcResult.nType);
			break;
		}
	}

	return TRUE;
}


LPCSTR CRedisAPI::GetLastErrorDesc()
{
	return m_szErrDesc;
}


LPCSTR CRedisAPI::GetLastCmdStr()
{
	return m_stRunCmd.strCommand.c_str();
}


int CRedisAPI::GetLastCmdID()
{
	return m_stRunCmd.nCommandID;
}


BOOL CRedisAPI::AddPipeCommand(int nRedisCmd, int nParamCount, ...)
{
	if (m_pContext == nullptr)
	{
		return FALSE;
	}

	// 命令
	_TRunCommand stCommand;
	stCommand.nCommandID = nRedisCmd;
	stCommand.strCommand.clear();

	// 解析参数
	vector<std::string>	vecParam;
	va_list param;
	va_start(param, nParamCount);
	for (int nIndex = 0; nIndex < nParamCount; ++nIndex)
	{
		vecParam.push_back(va_arg(param, char*));
	}
	va_end(param);

	// 构造命令字符串
	if (!m_oCommandData.FormatCmd(nRedisCmd, &vecParam[0], nParamCount, m_stRunCmd.strCommand))
	{
		__LogError("%s: %s", __FUNCTION__, m_oCommandData.GetLastErrDesc());
		return FALSE;
	}

	// 添加命令
	if (redisAppendCommand(m_pContext, m_stRunCmd.strCommand.c_str()) != REDIS_OK)
	{
		return FALSE;
	}

	m_vecCommand.push_back(stCommand);
	return TRUE;
}


BOOL CRedisAPI::RunPipeCommand()
{
	// 结果重置
	m_vecResult.clear();

	if (m_pContext == NULL)
	{
		return FALSE;
	}

	redisReply* pReply = NULL;
	for (int nIndex = 0; nIndex < m_vecCommand.size(); ++nIndex)
	{
		const string& strCommand = m_vecCommand[nIndex].strCommand;
		// 执行
		if (redisGetReply(m_pContext, (void**)&pReply) != REDIS_OK)
		{
			__LogError("%s: no reply results!  err:%d errstr:%s szCmd=%s", __FUNCTION__, m_pContext->err, m_pContext->errstr, strCommand.c_str());
			continue;
		}

		// 检查是否有错误
		if (pReply->type == REDIS_REPLY_ERROR)
		{
			__LogError("%s: redisGetReply has ERROR!  str:%s szCmd:%s", __FUNCTION__, pReply->str, strCommand.c_str());
			freeReplyObject(pReply);
			continue;
		}

		// 处理结果
		__HandleReply(pReply);
		// 释放结果集
		freeReplyObject(pReply);
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

BOOL CRedisAPI::Connect(const TRedisRunCfg& stRunCfg)
{
	if (__Ping())
	{
		return TRUE;
	}

	// 断开连接
	Disconnect();

	// 连接
	struct timeval tv = { 0, stRunCfg.nTimeOutInterval };
	m_pContext = redisConnectWithTimeout(stRunCfg.szIP, stRunCfg.dwPort, tv);
	if (m_pContext == NULL)
	{
		return FALSE;
	}

	if (stRunCfg.bHasPassword && !__Auth(stRunCfg.szPassword))
	{
		return FALSE;
	}

	if (!__Ping())
	{
		Disconnect();
		return FALSE;
	}

	return TRUE;
}


void CRedisAPI::Disconnect()
{
	if (m_pContext != NULL)
	{
		redisFree(m_pContext);
		m_pContext = NULL;
	}
}


//////////////////////////////////////////////////////////////////////////

BOOL CRedisAPI::__Auth(LPCSTR szPassword)
{
	char szCmd[100] = { 0 };
	sprintf_s(szCmd, sizeof(szCmd), "AUTH %s", szPassword);
	if (!__RunCommand(szCmd))
	{
		return FALSE;
	}

	// 检查回复
	if (m_vecResult.empty())
	{
		__LogError("%s: AUTH command has no reply!", __FUNCTION__);
		return FALSE;
	}

	const _TRunCmdResult& stResult = m_vecResult[0];
	if (strcmp(stResult.strResult.c_str(), "OK") != 0)
	{
		__LogError("%s: AUTH FAIL!  strResult=%s", __FUNCTION__, stResult.strResult.c_str());
		return FALSE;
	}

	// 清空回复
	m_vecResult.clear();
	return TRUE;
}


BOOL CRedisAPI::__Ping()
{
	if (!__RunCommand("PING"))
	{
		return FALSE;
	}

	// 检查回复
	if (m_vecResult.empty())
	{
		__LogError("%s: PING command has no reply!", __FUNCTION__);
		return FALSE;
	}

	const _TRunCmdResult& stResult = m_vecResult[0];
	if (strcmp(stResult.strResult.c_str(), "PONG") != 0)
	{
		__LogError("%s: Server reply wrong string for PING command!  strResult=%s", __FUNCTION__, stResult.strResult.c_str());
		return FALSE;
	}

	// 清空回复
	m_vecResult.clear();
	return TRUE;
}


BOOL CRedisAPI::__RunCommand(LPCSTR szCmd)
{
	// 结果重置
	m_vecResult.clear();

	if (m_pContext == NULL)
	{
		return FALSE;
	}

	// 结果
	BOOL bResult = TRUE;

	// 执行
	redisReply* pReply = (redisReply*)redisCommand(m_pContext, szCmd);
	if (pReply == NULL)
	{
		__LogError("%s: no reply results!  err:%d errstr:%s szCmd=%s", __FUNCTION__, m_pContext->err, m_pContext->errstr, szCmd);
		return FALSE;
	}

	// 检查是否有错误
	if (pReply->type == REDIS_REPLY_ERROR)
	{
		__LogError("%s: redisReply has ERROR!  str:%s szCmd:%s", __FUNCTION__, pReply->str, szCmd);
		bResult = FALSE;
		goto EXIT;
	}

	// 处理结果
	if (!__HandleReply(pReply))
	{
		bResult = FALSE;
		goto EXIT;
	}

EXIT:
	// 释放结果集
	freeReplyObject(pReply);
	return bResult;
}


BOOL CRedisAPI::__HandleReply(redisReply* pReply)
{
	switch (pReply->type)
	{
		case REDIS_REPLY_NIL:
		{
			return TRUE;
		}
		break;

		case REDIS_REPLY_ERROR:
		{
			__LogError("%s: Reply has error!  errstr=%s szCmd=%s", __FUNCTION__, pReply->str, GetLastCmdStr());
			return FALSE;
		}
		break;

		case REDIS_REPLY_STRING:
		case REDIS_REPLY_INTEGER:
		case REDIS_REPLY_STATUS:
		{
			_TRunCmdResult stResult;
			stResult.n64Value = pReply->integer;
			stResult.nType = pReply->type;
			if (pReply->type == REDIS_REPLY_STRING || pReply->type == REDIS_REPLY_STATUS)
			{
				stResult.strResult.assign(pReply->str);
			}
			// 保存
			m_vecResult.push_back(stResult);

			return TRUE;
		}
		break;

		case REDIS_REPLY_ARRAY:
		{
			if (pReply->element == NULL)
			{
				// 这里有可能没有结果，不算错误
				//__LogError("%s: REDIS_REPLY_ARRAY element is NULL!  type=%d  szCmd=%s", __FUNCTION__, pReply->type, szCmd);
				return TRUE;
			}

			for (int nIndex = 0; nIndex < pReply->elements; ++nIndex)
			{
				if (!__HandleReply(pReply->element[nIndex]))
				{
					return FALSE;
				}
			}

			return TRUE;
		}
		break;

		default:
		{
			__LogError("%s: Unknown reply type=%d szCmd=%s", __FUNCTION__, pReply->type, GetLastCmdStr());
			return FALSE;
		}
		break;
	}
}


void CRedisAPI::__LogError(const char* szFormat, ...)
{
	va_list param;
	va_start(param, szFormat);
	vsnprintf_s(m_szErrDesc, sizeof(m_szErrDesc), _TRUNCATE, szFormat, param);
	va_end(param);
}
