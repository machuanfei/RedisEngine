#pragma once


namespace RedisEngine
{
	// ���Ͷ���
	typedef DWORD HRTRANSID;				// ����ID
	const DWORD INVALID_HRTRANSID = 0;		// ��Ч����ID
	const DWORD CONTEXT_BUFFER_LEN = 1024;	// �ֳ���С

	// Redis�ظ����
	enum EnRedisReply
	{
		enReply_String = 1,
		enReply_Array,
		enReply_Integer,
		enReply_Nil,
		enReply_Status,
		enReply_Error,
	};


	// Redis����ö��
	enum EnRedisCommand
	{
		enCmdID_Min = 0,

		// String����
		enCmdID_Set,		// SET key value ����ָ�� key ��ֵ
		enCmdID_Get,			// GET key ��ȡָ�� key ��ֵ
		enCmdID_GetRange,		// GETRANGE key start end ���� key ���ַ���ֵ�����ַ�
		enCmdID_GetSet,		// GETSET key value ������ key ��ֵ��Ϊ value �������� key �ľ�ֵ(old value)
		enCmdID_GetBit,		// GETBIT key offset �� key ��������ַ���ֵ����ȡָ��ƫ�����ϵ�λ(bit)
		enCmdID_MGet,			// MGET key1 [key2..] ��ȡ����(һ������)���� key ��ֵ
		enCmdID_SetBit,		// SETBIT key offset value �� key ��������ַ���ֵ�����û����ָ��ƫ�����ϵ�λ(bit)
		enCmdID_SetEX,		// SETEX key seconds value ��ֵ value ������ key ������ key �Ĺ���ʱ����Ϊ seconds���룩
		enCmdID_SetNX,		// SETNX key value ֻ���� key ������ʱ���� key ��ֵ
		enCmdID_SetRange,		// SETRANGE key offset value �� value ������д���� key ��������ַ���ֵ����ƫ���� offset ��ʼ
		enCmdID_Strlen,		// STRLEN key ���� key ��������ַ���ֵ�ĳ���
		enCmdID_MSet,			// MSET key value [key value ...] ͬʱ����һ������ key-value ��
		enCmdID_MSetNX,		// MSETNX key value [key value ...] ͬʱ����һ������ key-value �ԣ����ҽ������и��� key ��������
		enCmdID_PSetEX,		// PSETEX key milliseconds value ��ֵ value ������ key������ key �Ĺ���ʱ����Ϊ milliseconds�����룩
		enCmdID_Incr,			// INCR key �� key �д��������ֵ��һ
		enCmdID_IncrBy,		// INCRBY key increment �� key �������ֵ���ϸ���������ֵ��increment��
		enCmdID_IncrByFloat,	// INCRBYFLOAT key increment �� key �������ֵ���ϸ����ĸ�������ֵ��increment��
		enCmdID_Decr,			// DECR key �� key �д��������ֵ��һ
		enCmdID_DecrBy,		// DECRBY key decrement key �������ֵ��ȥ�����ļ���ֵ��decrement��
		enCmdID_Append,		// APPEND key value ��� key �Ѿ����ڲ�����һ���ַ����� APPEND ��� value ׷�ӵ� key ԭ����ֵ��ĩβ

		// Keys����
		enCmdID_Del,			// DEL key  key ����ʱɾ�� key
		enCmdID_Dump,			// DUMP key ���л����� key �������ر����л���ֵ
		enCmdID_Exists,		// EXISTS key ������ key �Ƿ����
		enCmdID_Expire,		// EXPIRE key seconds Ϊ���� key ���ù���ʱ��
		enCmdID_ExpireAt,		// EXPIREAT key timestamp Ϊ���� key ���ù���ʱ�䣨ʱ���(unix timestamp)��
		enCmdID_PExpire,		// PEXPIRE key milliseconds ���� key �Ĺ���ʱ���Ժ����
		enCmdID_PExpireAt,	// PEXPIREAT key milliseconds-timestamp ���� key ����ʱ���ʱ���(unix timestamp) �Ժ����
		enCmdID_Keys,			// KEYS pattern �������з��ϸ���ģʽ( pattern)�� key 
		enCmdID_Move,			// MOVE key db ����ǰ���ݿ�� key �ƶ������������ݿ� db ����
		enCmdID_Persist,		// PERSIST key �Ƴ� key �Ĺ���ʱ�䣬key ���־ñ���
		enCmdID_PTTL,			// PTTL key �Ժ���Ϊ��λ���� key ��ʣ��Ĺ���ʱ��
		enCmdID_TTL,			// TTL key ����Ϊ��λ�����ظ��� key ��ʣ������ʱ��(TTL, time to live)��
		enCmdID_Rename,		// RENAME key newkey �޸� key ������
		enCmdID_RenameNX,		// RENAMENX key newkey ���� newkey ������ʱ���� key ����Ϊ newkey
		enCmdID_Type,			// TYPE key ���� key �������ֵ������

		// List����
		enCmdID_LIndex,		// LINDEX key index ͨ��������ȡ�б��е�Ԫ��
		enCmdID_LInsert,		// LINSERT key BEFORE|AFTER pivot value  ���б��Ԫ��ǰ���ߺ����Ԫ��
		enCmdID_LLen,			// LLEN key ��ȡ�б���
		enCmdID_LPush,		// LPUSH key value1 [value2]  ��һ������ֵ���뵽�б�ͷ��
		enCmdID_LPop,			// LPOP key  �Ƴ�����ȡ�б�ĵ�һ��Ԫ��
		enCmdID_LPushX,		// LPUSHX key value ��һ��ֵ���뵽�Ѵ��ڵ��б�ͷ��
		enCmdID_LRange,		// LRANGE key start stop ��ȡ�б�ָ����Χ�ڵ�Ԫ��
		enCmdID_LRem,			// LREM key count value �Ƴ��б�Ԫ��
		enCmdID_LSet,			// LSET key index value ͨ�����������б�Ԫ�ص�ֵ
		enCmdID_LTrim,		// LTRIM key start stop ��һ���б�����޼�(trim)������˵�����б�ֻ����ָ�������ڵ�Ԫ�أ�����ָ������֮�ڵ�Ԫ�ض�����ɾ����
		enCmdID_RPop,			// RPOP key  �Ƴ�����ȡ�б����һ��Ԫ��
		enCmdID_RPopLPush,	// RPOPLPUSH source destination �Ƴ��б�����һ��Ԫ�أ�������Ԫ����ӵ���һ���б�����
		enCmdID_RPush,		// RPUSH key value1 [value2] ���б������һ������ֵ
		enCmdID_RPushX,		// RPUSHX key value1 [value2] Ϊ�Ѵ��ڵ��б����ֵ

		// Hash����
		enCmdID_HDel,		// HDEL key field1 [field2] ɾ��һ��������ϣ���ֶ�
		enCmdID_HExists,	// HEXISTS key field �鿴��ϣ�� key �У�ָ�����ֶ��Ƿ����
		enCmdID_HGet,		// HGET key field ��ȡ�洢�ڹ�ϣ����ָ���ֶε�ֵ
		enCmdID_HGetAll,	// HGETALL key ��ȡ�ڹ�ϣ����ָ�� key �������ֶκ�ֵ
		enCmdID_HIncrBy,	// HINCRBY key field increment Ϊ��ϣ�� key �е�ָ���ֶε�����ֵ�������� increment 
		enCmdID_HIncrByFloat,	// HINCRBYFLOAT key field increment Ϊ��ϣ�� key �е�ָ���ֶεĸ�����ֵ�������� increment
		enCmdID_HKeys,		// HKEYS key ��ȡ���й�ϣ���е��ֶ�
		enCmdID_HLen,		// HLEN key ����
		enCmdID_HMGet,		// HMGET key field1 [field2] ��ȡ���и����ֶε�ֵ
		enCmdID_HMSet,		// HMSET key field1 value1 [field2 value2 ] ͬʱ����� field-value (��-ֵ)�����õ���ϣ�� key ��
		enCmdID_HSet,		// HSET key field value ����ϣ�� key �е��ֶ� field ��ֵ��Ϊ value
		enCmdID_HSetNX,		// HSETNX key field value ֻ�����ֶ� field ������ʱ�����ù�ϣ���ֶε�ֵ
		enCmdID_HVals,		// HVALS key ��ȡ��ϣ��������ֵ

		//Set����
		enCmdID_SAdd,		// SADD key member1 [member2] �򼯺����һ��������Ա
		enCmdID_SRem,		// SREM key member1 [member2] �Ƴ�������һ��������Ա
		enCmdID_SCard,		// SCARD key  ��ȡ���ϵĳ�Ա��
		enCmdID_SDiff,		// SDIFF key1 [key2] ���ظ������м��ϵĲ
		enCmdID_SInter,		// SINTER key1 [key2] ���ظ������м��ϵĽ���
		enCmdID_SIsMember,	// SISMEMBER key member �ж� member Ԫ���Ƿ��Ǽ��� key �ĳ�Ա
		enCmdID_SMembers,	// SMEMBERS key ���ؼ����е����г�Ա
		enCmdID_SUnion,		// SUNION key1 [key2] �������и������ϵĲ���
		enCmdID_SScan,		// SSCAN key cursor [MATCH pattern] [COUNT count] ���������е�Ԫ��  cursor��ʾ��ʼλ��

		enCmdID_Max,
	};


	// Redis��������
	struct TRedisRunCfg
	{
		enum
		{
			IP_LEN_MAX = 32,	// IP��󳤶�
			PASSWORD_LEN_MAX = 32,	// ������󳤶�
		};


		char	szPassword[PASSWORD_LEN_MAX];	// ����
		char	szIP[IP_LEN_MAX];
		bool	bHasPassword;		// �Ƿ�������
		DWORD	dwPort;
		DWORD	dwThreadNum;		// �����̼߳����߳�����
		DWORD	dwSuiteNum;			// �̼߳�����
		int		nTimeOutInterval;	// ��ʱʱ��(����)
	};


	// ִ�н��
	struct TRunCmdResult
	{
		int type;			/* EnRedisReply */
		__int64 integer;	/* The integer when type is enReply_Integer */
		int len;			/* Length of string */
		char* str;			/* Used for both enReply_Error and enReply_String */
	};


	// Redis Engine �ṩ��API�ӿ�
	struct IRedisAPI
	{
		// ִ������
		// ÿ��ִ����������ý����
		virtual BOOL RunCommand(int nRedisCmd, int nParamCount, ...) = 0;

		// ��ȡ�������
		virtual int GetResultCount() = 0;

		// ��ȡ���
		virtual BOOL GetResult(TRunCmdResult* pResult, int nCount) = 0;

		// ��ȡ��һ�β�����������
		virtual LPCSTR GetLastErrorDesc() = 0;

		// ��ȡ��һ��ִ�����
		virtual LPCSTR GetLastCmdStr() = 0;

		// ��ȡ��һ��ִ������ID
		virtual int GetLastCmdID() = 0;

		// �ܵ���ִ�ж�������
		virtual BOOL AddPipeCommand(int nRedisCmd, int nParamCount, ...) = 0;

		// ִ�йܵ�����
		virtual BOOL RunPipeCommand() = 0;
	};


	// Redis ִ������ӿ�
	struct IRedisTask
	{
		// ִ��
		virtual void Do(IRedisAPI* pRedisAPI) = 0;

		// ��ȡ��������
		virtual int GetTaskType() = 0;

		// ��ȡִ�н��
		virtual LPCSTR GetRunResult() = 0;

		// ��ȡ�ֳ���С
		virtual int GetContextLen() = 0;

		// �������
		virtual bool ExportResult(LPSTR pszContext, int nLen) = 0;
	};


	// �ص��ӿ�
	struct TRedisResponse;
	struct IRedisTaskRetSink
	{
		// �ص�
		virtual void OnRedisTaskRet(HRTRANSID hAID, const TRedisResponse* pResponse) = 0;
	};


	// ����ṹ
	struct TRedisRequest
	{
		HRTRANSID				hActID;			// ����ID
		IRedisTask*				pTask;			// ����
		IRedisTaskRetSink*		pRetSink;		// �ص��ӿ�
		int						nSuiteID;		// �̼߳�ID
	};


	// �ظ��ṹ
	struct TRedisResponse
	{
		HRTRANSID				hActID;			// ����ID
		int						nTaskType;		// ��������
		char					arrContext[CONTEXT_BUFFER_LEN];	// �ֳ�
		int						nContextLen;	// �ֳ���С
	};


	// RedisEngine
	struct IRedisEngine
	{
		// ����
		virtual BOOL Create(TRedisRunCfg* pRunCfg) = 0;

		// �ͷ�
		virtual void Release() = 0;

		// ��������
		virtual BOOL StartService() = 0;

		// ֹͣ����
		virtual BOOL StopService(DWORD dwWaitTime) = 0;

		// ȡ������
		virtual BOOL CancelRequest(HRTRANSID hAID) = 0;

		// ��ȡ��һ�β�����������
		virtual LPCSTR GetLastErrorMsg() = 0;

		// ����
		virtual HRTRANSID Request(IRedisTask* pTask, IRedisTaskRetSink* pRetSink, int nSuiteID) = 0;

		// �����Ѿ�ִ����ɵ������ⲿ�ǵõ������������
		virtual BOOL PopFinishedResult() = 0;

		// ��ȡ��Ϣ
		virtual BOOL PopRuntimeMsg(LPSTR pBuffer, DWORD dwBufferLen) = 0;
	};
};
