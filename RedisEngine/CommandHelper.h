#pragma once

class CCommandHelper
{
public:
	CCommandHelper();
	~CCommandHelper();

	// 格式化命令
	bool FormatCmd(int nCmdID, const std::string* arrParam, int nParamCount, std::string& strFormat);

	// 获取错误描述
	LPCSTR GetLastErrDesc();


private:
	struct _TCommandData;

	typedef bool (* CHECK_PARAM_FUNC)(const _TCommandData* pCmdData, const std::string* arrParam, int nParamCount);
	typedef void (* FORMAT_CMD_FUNC)(const _TCommandData* pCmdData, const std::string* arrParam, int nParamCount, std::string& strFormat);

	struct _TCommandData
	{
		int nCmdID;							// 命令ID
		char szPattern[128];				// 格式化串
		int nParamCountMax;					// 最多参数数量
		int nParamCountMin;					// 最少参数数量
		CHECK_PARAM_FUNC funcCheckParam;	// 检查参数函数
		FORMAT_CMD_FUNC funcFormatCmd;		// 格式化参数
	};

	struct _TCommandCfg
	{
		LPCSTR szPattern;
		int nParamCountMax;
		int nParamCountMin;
	};

private:
	// 常规参数检查，只检查数量
	static bool __CheckParamFuncNormal(const _TCommandData* pCmdData, const std::string* arrParam, int nParamCount);

	// 常规格式化
	static void __FormatCmdNormal(const _TCommandData* pCmdData, const std::string* arrParam, int nParamCount, std::string& strFormat);

	// 成对Key/Value检查
	static bool __CheckParamFuncPair(const _TCommandData* pCmdData, const std::string* arrParam, int nParamCount);

	// LInsert格式化
	static void __FormatCmdForLInsert(const _TCommandData* pCmdData, const std::string* arrParam, int nParamCount, std::string& strFormat);

	// SSCAN格式化
	static void __FormatCmdForSScan(const _TCommandData* pCmdData, const std::string* arrParam, int nParamCount, std::string& strFormat);

	// 记录错误
	void __LogError(const char* szFormat, ...);
private:
	// 命令数据列表
	static const _TCommandData	s_arrCommandData[enCmdID_Max];

	// 错误描述
	char						m_szErrDesc[2048];
};
