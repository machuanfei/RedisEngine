#include "stdafx.h"
#include "CommandHelper.h"


// ��������
const CCommandHelper::_TCommandData	CCommandHelper::s_arrCommandData[enCmdID_Max] = 
{
	{enCmdID_Min, "", 0, 0, NULL, NULL},

	// String����
	{ enCmdID_Set, "SET", 2, 2, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_Get, "GET", 1, 1, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_GetRange, "GETRANGE", 3, 3, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_GetSet, "GETSET", 2, 2, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_GetBit, "GETBIT", 2, 2, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_MGet, "MGET", -1, 1, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_SetBit, "SETBIT", 3, 3, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_SetEX, "SETEX", 3, 3, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },	
	{ enCmdID_SetNX, "SETNX", 2, 2, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },	
	{ enCmdID_SetRange, "SETRANGE", 3, 3, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_Strlen, "STRLEN", 1, 1, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_MSet, "MSET", -1, 2, &CCommandHelper::__CheckParamFuncPair, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_MSetNX, "MSETNX", -1, 2, &CCommandHelper::__CheckParamFuncPair, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_PSetEX, "PSETEX", 3, 3, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_Incr, "INCR", 1, 1, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_IncrBy, "INCRBY", 2, 2, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_IncrByFloat, "INCRBYFLOAT", 2, 2, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_Decr, "DECR", 1, 1, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_DecrBy, "DECRBY", 2, 2, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_Append, "APPEND", 2, 2, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },

	// Key����
	{ enCmdID_Del, "DEL", 1, 1, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_Dump, "DUMP", 1, 1, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_Exists, "EXISTS", 1, 1, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_Expire, "EXPIRE", 2, 2, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_ExpireAt, "EXPIREAT", 2, 2, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_PExpire, "PEXPIRE", 2, 2, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_PExpireAt, "PEXPIREAT", 2, 2, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_Keys, "KEYS", 1, 1, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_Move, "MOVE", 2, 2, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_Persist, "PERSIST", 1, 1, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_PTTL, "PTTL", 1, 1, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_TTL, "TTL", 1, 1, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_Rename, "RENAME", 2, 2, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_RenameNX, "RENAMENX", 2, 2, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_Type, "TYPE", 1, 1, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },

	// List����
	{ enCmdID_LIndex, "LINDEX", 2, 2, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_LInsert, "LINSERT", 4, 4, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdForLInsert },
	{ enCmdID_LLen, "LLEN", 1, 1, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_LPush, "LPUSH", -1, 2, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_LPop, "LPOP", 1, 1, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_LPushX, "LPUSHX", 2, 2, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_LRange, "LRANGE", 3, 3, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_LRem, "LREM", 3, 3, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_LSet, "LSET", 3, 3, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_LTrim, "LTRIM", 3, 3, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_RPop, "RPOP", 1, 1, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_RPopLPush, "RPOPLPUSH", 2, 2, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_RPush, "RPUSH", -1, 2, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_RPushX, "RPUSHX", -1, 2, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },

	// Hash����
	{ enCmdID_HDel, "HDEL", -1, 2, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_HExists, "HEXISTS", 2, 2, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdForLInsert },
	{ enCmdID_HGet, "HGET", 2, 2, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_HGetAll, "HGETALL", 1, 1, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_HIncrBy, "HINCRBY", 3, 3, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_HIncrByFloat, "HINCRBYFLOAT", 3, 3, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_HKeys, "HKEYS", 1, 1, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_HLen, "HLEN", 1, 1, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_HMGet, "HMGET", -1, 2, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_HMSet, "HMSET", -1, 3, &CCommandHelper::__CheckParamFuncPair, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_HSet, "HSET", 3, 3, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_HSetNX, "HSETNX", 3, 3, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_HVals, "HVALS", 1, 1, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },

	//Set����
	{ enCmdID_SAdd, "SADD", -1, 2, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_SRem, "SREM", -1, 2, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_SCard, "SCARD", 1, 1, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdForLInsert },
	{ enCmdID_SDiff, "SDIFF", -1, 1, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_SInter, "SINTER", 1, 1, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_SIsMember, "SISMEMBER", 2, 2, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_SMembers, "SMEMBERS", 1, 1, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_SUnion, "SUNION", -1, 1, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdNormal },
	{ enCmdID_SScan, "SSCAN", 4, 2, &CCommandHelper::__CheckParamFuncNormal, &CCommandHelper::__FormatCmdForSScan },
};


CCommandHelper::CCommandHelper()
{
	
}


CCommandHelper::~CCommandHelper()
{
}


bool CCommandHelper::FormatCmd(int nCmdID, const std::string* arrParam, int nParamCount, std::string& strFormat)
{
	if (nCmdID <= enCmdID_Min && nCmdID >= enCmdID_Max)
	{
		__LogError("%s: CommandID is INVALID!  nCmdID=%d", __FUNCTION__, nCmdID);
		return false;
	}

	const _TCommandData& stCmdData = s_arrCommandData[nCmdID];

	// ������
	CHECK_PARAM_FUNC funcCheckParam = stCmdData.funcCheckParam;
	if (funcCheckParam != NULL)
	{
		if (!(*funcCheckParam)(&stCmdData, arrParam, nParamCount))
		{
			__LogError("%s: nParamCount=%d INCORRECT!  nCmdID=%d  nParamMin=%d  nParamMax=%d", __FUNCTION__, nParamCount, nCmdID, stCmdData.nParamCountMin, stCmdData.nParamCountMax);
			return false;
		}
	}

	// ִ�и�ʽ��
	FORMAT_CMD_FUNC funcFormatCmd = stCmdData.funcFormatCmd;
	if (funcFormatCmd == NULL)
	{
		__LogError("%s: funcFormatCmd is NULL!  nCmdID=%d", __FUNCTION__, nCmdID);
		return false;
	}

	(*funcFormatCmd)(&stCmdData, arrParam, nParamCount, strFormat);
	return true;
}


LPCSTR CCommandHelper::GetLastErrDesc()
{
	return m_szErrDesc;
}


bool CCommandHelper::__CheckParamFuncNormal(const _TCommandData* pCmdData, const std::string* arrParam, int nParamCount)
{
	int nParamMin = pCmdData->nParamCountMin;
	int nParamMax = pCmdData->nParamCountMax;
	// nParamMax == -1��ʾ�����Ʋ�������
	if (nParamCount < nParamMin || (nParamMax != -1 && nParamCount > nParamMax))
	{
		return false;
	}

	return true;
}


void CCommandHelper::__FormatCmdNormal(const _TCommandData* pCmdData, const std::string* arrParam, int nParamCount, std::string& strFormat)
{
	strFormat.assign(pCmdData->szPattern);

	for (int nIndex = 0; nIndex < nParamCount; ++nIndex)
	{
		strFormat.append(" ");
		strFormat.append(arrParam[nIndex]);
	}
}


bool CCommandHelper::__CheckParamFuncPair(const _TCommandData* pCmdData, const std::string* arrParam, int nParamCount)
{
	int nParamMin = pCmdData->nParamCountMin;
	int nParamMax = pCmdData->nParamCountMax;
	// nParamMax == -1��ʾ�����Ʋ�������
	if (nParamCount < nParamMin || (nParamMax != -1 && nParamCount > nParamMax))
	{
		return false;
	}

	// ��ȥKeyʣ�µĲ���ӦΪż��
	if ((nParamCount - 1) % 2 != 0)
	{
		return false;
	}

	return true;
}


void CCommandHelper::__FormatCmdForLInsert(const _TCommandData* pCmdData, const std::string* arrParam, int nParamCount, std::string& strFormat)
{
	strFormat.assign(pCmdData->szPattern);

	for (int nIndex = 0; nIndex < nParamCount; ++nIndex)
	{
		if (nIndex == 1)
		{
			// ��һ������ΪBEFORE/AFTER   1ΪAFTER  ����ΪBEFORE
			int nValue = atoi(arrParam[nIndex].c_str());
			LPCSTR szParam = ((nValue == 1) ? "AFTER" : "BEFORE");
			strFormat.append(" ");
			strFormat.append(szParam);
			continue;
		}

		strFormat.append(" ");
		strFormat.append(arrParam[nIndex]);
	}
}


void CCommandHelper::__FormatCmdForSScan(const _TCommandData* pCmdData, const std::string* arrParam, int nParamCount, std::string& strFormat)
{
	strFormat.assign(pCmdData->szPattern);

	for (int nIndex = 0; nIndex < nParamCount; ++nIndex)
	{
		if (nIndex == 2)
		{
			// ������������Ҫ����MATCH
			strFormat.append(" MATCH ");
			strFormat.append(arrParam[nIndex]);
			continue;
		}
		else if (nIndex == 3)
		{
			// ������������Ҫ����MATCH
			strFormat.append(" COUNT ");
			strFormat.append(arrParam[nIndex]);
			continue;
		}

		strFormat.append(" ");
		strFormat.append(arrParam[nIndex]);
	}
}


void CCommandHelper::__LogError(const char* szFormat, ...)
{
	va_list param;
	va_start(param, szFormat);
	vsnprintf_s(m_szErrDesc, sizeof(m_szErrDesc), _TRUNCATE, szFormat, param);
	va_end(param);
}

