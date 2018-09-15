#pragma once

class CCommandHelper
{
public:
	CCommandHelper();
	~CCommandHelper();

	// ��ʽ������
	bool FormatCmd(int nCmdID, const std::string* arrParam, int nParamCount, std::string& strFormat);

	// ��ȡ��������
	LPCSTR GetLastErrDesc();


private:
	struct _TCommandData;

	typedef bool (* CHECK_PARAM_FUNC)(const _TCommandData* pCmdData, const std::string* arrParam, int nParamCount);
	typedef void (* FORMAT_CMD_FUNC)(const _TCommandData* pCmdData, const std::string* arrParam, int nParamCount, std::string& strFormat);

	struct _TCommandData
	{
		int nCmdID;							// ����ID
		char szPattern[128];				// ��ʽ����
		int nParamCountMax;					// ����������
		int nParamCountMin;					// ���ٲ�������
		CHECK_PARAM_FUNC funcCheckParam;	// ����������
		FORMAT_CMD_FUNC funcFormatCmd;		// ��ʽ������
	};

	struct _TCommandCfg
	{
		LPCSTR szPattern;
		int nParamCountMax;
		int nParamCountMin;
	};

private:
	// ���������飬ֻ�������
	static bool __CheckParamFuncNormal(const _TCommandData* pCmdData, const std::string* arrParam, int nParamCount);

	// �����ʽ��
	static void __FormatCmdNormal(const _TCommandData* pCmdData, const std::string* arrParam, int nParamCount, std::string& strFormat);

	// �ɶ�Key/Value���
	static bool __CheckParamFuncPair(const _TCommandData* pCmdData, const std::string* arrParam, int nParamCount);

	// LInsert��ʽ��
	static void __FormatCmdForLInsert(const _TCommandData* pCmdData, const std::string* arrParam, int nParamCount, std::string& strFormat);

	// SSCAN��ʽ��
	static void __FormatCmdForSScan(const _TCommandData* pCmdData, const std::string* arrParam, int nParamCount, std::string& strFormat);

	// ��¼����
	void __LogError(const char* szFormat, ...);
private:
	// ���������б�
	static const _TCommandData	s_arrCommandData[enCmdID_Max];

	// ��������
	char						m_szErrDesc[2048];
};
