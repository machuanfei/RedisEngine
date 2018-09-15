#pragma once


namespace RedisEngine
{
	// 类型定义
	typedef DWORD HRTRANSID;				// 事务ID
	const DWORD INVALID_HRTRANSID = 0;		// 无效事务ID
	const DWORD CONTEXT_BUFFER_LEN = 1024;	// 现场大小

	// Redis回复结果
	enum EnRedisReply
	{
		enReply_String = 1,
		enReply_Array,
		enReply_Integer,
		enReply_Nil,
		enReply_Status,
		enReply_Error,
	};


	// Redis命令枚举
	enum EnRedisCommand
	{
		enCmdID_Min = 0,

		// String命令
		enCmdID_Set,		// SET key value 设置指定 key 的值
		enCmdID_Get,			// GET key 获取指定 key 的值
		enCmdID_GetRange,		// GETRANGE key start end 返回 key 中字符串值的子字符
		enCmdID_GetSet,		// GETSET key value 将给定 key 的值设为 value ，并返回 key 的旧值(old value)
		enCmdID_GetBit,		// GETBIT key offset 对 key 所储存的字符串值，获取指定偏移量上的位(bit)
		enCmdID_MGet,			// MGET key1 [key2..] 获取所有(一个或多个)给定 key 的值
		enCmdID_SetBit,		// SETBIT key offset value 对 key 所储存的字符串值，设置或清除指定偏移量上的位(bit)
		enCmdID_SetEX,		// SETEX key seconds value 将值 value 关联到 key ，并将 key 的过期时间设为 seconds（秒）
		enCmdID_SetNX,		// SETNX key value 只有在 key 不存在时设置 key 的值
		enCmdID_SetRange,		// SETRANGE key offset value 用 value 参数覆写给定 key 所储存的字符串值，从偏移量 offset 开始
		enCmdID_Strlen,		// STRLEN key 返回 key 所储存的字符串值的长度
		enCmdID_MSet,			// MSET key value [key value ...] 同时设置一个或多个 key-value 对
		enCmdID_MSetNX,		// MSETNX key value [key value ...] 同时设置一个或多个 key-value 对，当且仅当所有给定 key 都不存在
		enCmdID_PSetEX,		// PSETEX key milliseconds value 将值 value 关联到 key，并将 key 的过期时间设为 milliseconds（毫秒）
		enCmdID_Incr,			// INCR key 将 key 中储存的数字值增一
		enCmdID_IncrBy,		// INCRBY key increment 将 key 所储存的值加上给定的增量值（increment）
		enCmdID_IncrByFloat,	// INCRBYFLOAT key increment 将 key 所储存的值加上给定的浮点增量值（increment）
		enCmdID_Decr,			// DECR key 将 key 中储存的数字值减一
		enCmdID_DecrBy,		// DECRBY key decrement key 所储存的值减去给定的减量值（decrement）
		enCmdID_Append,		// APPEND key value 如果 key 已经存在并且是一个字符串， APPEND 命令将 value 追加到 key 原来的值的末尾

		// Keys命令
		enCmdID_Del,			// DEL key  key 存在时删除 key
		enCmdID_Dump,			// DUMP key 序列化给定 key ，并返回被序列化的值
		enCmdID_Exists,		// EXISTS key 检查给定 key 是否存在
		enCmdID_Expire,		// EXPIRE key seconds 为给定 key 设置过期时间
		enCmdID_ExpireAt,		// EXPIREAT key timestamp 为给定 key 设置过期时间（时间戳(unix timestamp)）
		enCmdID_PExpire,		// PEXPIRE key milliseconds 设置 key 的过期时间以毫秒计
		enCmdID_PExpireAt,	// PEXPIREAT key milliseconds-timestamp 设置 key 过期时间的时间戳(unix timestamp) 以毫秒计
		enCmdID_Keys,			// KEYS pattern 查找所有符合给定模式( pattern)的 key 
		enCmdID_Move,			// MOVE key db 将当前数据库的 key 移动到给定的数据库 db 当中
		enCmdID_Persist,		// PERSIST key 移除 key 的过期时间，key 将持久保持
		enCmdID_PTTL,			// PTTL key 以毫秒为单位返回 key 的剩余的过期时间
		enCmdID_TTL,			// TTL key 以秒为单位，返回给定 key 的剩余生存时间(TTL, time to live)。
		enCmdID_Rename,		// RENAME key newkey 修改 key 的名称
		enCmdID_RenameNX,		// RENAMENX key newkey 仅当 newkey 不存在时，将 key 改名为 newkey
		enCmdID_Type,			// TYPE key 返回 key 所储存的值的类型

		// List命令
		enCmdID_LIndex,		// LINDEX key index 通过索引获取列表中的元素
		enCmdID_LInsert,		// LINSERT key BEFORE|AFTER pivot value  在列表的元素前或者后插入元素
		enCmdID_LLen,			// LLEN key 获取列表长度
		enCmdID_LPush,		// LPUSH key value1 [value2]  将一个或多个值插入到列表头部
		enCmdID_LPop,			// LPOP key  移出并获取列表的第一个元素
		enCmdID_LPushX,		// LPUSHX key value 将一个值插入到已存在的列表头部
		enCmdID_LRange,		// LRANGE key start stop 获取列表指定范围内的元素
		enCmdID_LRem,			// LREM key count value 移除列表元素
		enCmdID_LSet,			// LSET key index value 通过索引设置列表元素的值
		enCmdID_LTrim,		// LTRIM key start stop 对一个列表进行修剪(trim)，就是说，让列表只保留指定区间内的元素，不在指定区间之内的元素都将被删除。
		enCmdID_RPop,			// RPOP key  移除并获取列表最后一个元素
		enCmdID_RPopLPush,	// RPOPLPUSH source destination 移除列表的最后一个元素，并将该元素添加到另一个列表并返回
		enCmdID_RPush,		// RPUSH key value1 [value2] 在列表中添加一个或多个值
		enCmdID_RPushX,		// RPUSHX key value1 [value2] 为已存在的列表添加值

		// Hash命令
		enCmdID_HDel,		// HDEL key field1 [field2] 删除一个或多个哈希表字段
		enCmdID_HExists,	// HEXISTS key field 查看哈希表 key 中，指定的字段是否存在
		enCmdID_HGet,		// HGET key field 获取存储在哈希表中指定字段的值
		enCmdID_HGetAll,	// HGETALL key 获取在哈希表中指定 key 的所有字段和值
		enCmdID_HIncrBy,	// HINCRBY key field increment 为哈希表 key 中的指定字段的整数值加上增量 increment 
		enCmdID_HIncrByFloat,	// HINCRBYFLOAT key field increment 为哈希表 key 中的指定字段的浮点数值加上增量 increment
		enCmdID_HKeys,		// HKEYS key 获取所有哈希表中的字段
		enCmdID_HLen,		// HLEN key 长度
		enCmdID_HMGet,		// HMGET key field1 [field2] 获取所有给定字段的值
		enCmdID_HMSet,		// HMSET key field1 value1 [field2 value2 ] 同时将多个 field-value (域-值)对设置到哈希表 key 中
		enCmdID_HSet,		// HSET key field value 将哈希表 key 中的字段 field 的值设为 value
		enCmdID_HSetNX,		// HSETNX key field value 只有在字段 field 不存在时，设置哈希表字段的值
		enCmdID_HVals,		// HVALS key 获取哈希表中所有值

		//Set命令
		enCmdID_SAdd,		// SADD key member1 [member2] 向集合添加一个或多个成员
		enCmdID_SRem,		// SREM key member1 [member2] 移除集合中一个或多个成员
		enCmdID_SCard,		// SCARD key  获取集合的成员数
		enCmdID_SDiff,		// SDIFF key1 [key2] 返回给定所有集合的差集
		enCmdID_SInter,		// SINTER key1 [key2] 返回给定所有集合的交集
		enCmdID_SIsMember,	// SISMEMBER key member 判断 member 元素是否是集合 key 的成员
		enCmdID_SMembers,	// SMEMBERS key 返回集合中的所有成员
		enCmdID_SUnion,		// SUNION key1 [key2] 返回所有给定集合的并集
		enCmdID_SScan,		// SSCAN key cursor [MATCH pattern] [COUNT count] 迭代集合中的元素  cursor表示起始位置

		enCmdID_Max,
	};


	// Redis连接配置
	struct TRedisRunCfg
	{
		enum
		{
			IP_LEN_MAX = 32,	// IP最大长度
			PASSWORD_LEN_MAX = 32,	// 密码最大长度
		};


		char	szPassword[PASSWORD_LEN_MAX];	// 密码
		char	szIP[IP_LEN_MAX];
		bool	bHasPassword;		// 是否有密码
		DWORD	dwPort;
		DWORD	dwThreadNum;		// 单个线程集中线程数量
		DWORD	dwSuiteNum;			// 线程集数量
		int		nTimeOutInterval;	// 超时时间(毫秒)
	};


	// 执行结果
	struct TRunCmdResult
	{
		int type;			/* EnRedisReply */
		__int64 integer;	/* The integer when type is enReply_Integer */
		int len;			/* Length of string */
		char* str;			/* Used for both enReply_Error and enReply_String */
	};


	// Redis Engine 提供的API接口
	struct IRedisAPI
	{
		// 执行命令
		// 每次执行命令都会重置结果集
		virtual BOOL RunCommand(int nRedisCmd, int nParamCount, ...) = 0;

		// 获取结果数量
		virtual int GetResultCount() = 0;

		// 获取结果
		virtual BOOL GetResult(TRunCmdResult* pResult, int nCount) = 0;

		// 获取上一次操作错误描述
		virtual LPCSTR GetLastErrorDesc() = 0;

		// 获取上一次执行语句
		virtual LPCSTR GetLastCmdStr() = 0;

		// 获取上一次执行命令ID
		virtual int GetLastCmdID() = 0;

		// 管道：执行多条命令
		virtual BOOL AddPipeCommand(int nRedisCmd, int nParamCount, ...) = 0;

		// 执行管道命令
		virtual BOOL RunPipeCommand() = 0;
	};


	// Redis 执行任务接口
	struct IRedisTask
	{
		// 执行
		virtual void Do(IRedisAPI* pRedisAPI) = 0;

		// 获取任务类型
		virtual int GetTaskType() = 0;

		// 获取执行结果
		virtual LPCSTR GetRunResult() = 0;

		// 获取现场大小
		virtual int GetContextLen() = 0;

		// 导出结果
		virtual bool ExportResult(LPSTR pszContext, int nLen) = 0;
	};


	// 回调接口
	struct TRedisResponse;
	struct IRedisTaskRetSink
	{
		// 回调
		virtual void OnRedisTaskRet(HRTRANSID hAID, const TRedisResponse* pResponse) = 0;
	};


	// 请求结构
	struct TRedisRequest
	{
		HRTRANSID				hActID;			// 事务ID
		IRedisTask*				pTask;			// 任务
		IRedisTaskRetSink*		pRetSink;		// 回调接口
		int						nSuiteID;		// 线程集ID
	};


	// 回复结构
	struct TRedisResponse
	{
		HRTRANSID				hActID;			// 事务ID
		int						nTaskType;		// 任务类型
		char					arrContext[CONTEXT_BUFFER_LEN];	// 现场
		int						nContextLen;	// 现场大小
	};


	// RedisEngine
	struct IRedisEngine
	{
		// 创建
		virtual BOOL Create(TRedisRunCfg* pRunCfg) = 0;

		// 释放
		virtual void Release() = 0;

		// 启动服务
		virtual BOOL StartService() = 0;

		// 停止服务
		virtual BOOL StopService(DWORD dwWaitTime) = 0;

		// 取消请求
		virtual BOOL CancelRequest(HRTRANSID hAID) = 0;

		// 获取上一次操作错误描述
		virtual LPCSTR GetLastErrorMsg() = 0;

		// 请求
		virtual HRTRANSID Request(IRedisTask* pTask, IRedisTaskRetSink* pRetSink, int nSuiteID) = 0;

		// 处理已经执行完成的请求（外部记得调用这个函数）
		virtual BOOL PopFinishedResult() = 0;

		// 获取消息
		virtual BOOL PopRuntimeMsg(LPSTR pBuffer, DWORD dwBufferLen) = 0;
	};
};
