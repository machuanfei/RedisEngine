#include "stdafx.h"
#include "TestTask.h"

CTestTask::CTestTask()
{
}


CTestTask::~CTestTask()
{
}


void CTestTask::Do(IRedisAPI* pRedisAPI)
{
	char szTest[100] = { 0 };
	//_stprintf_s(szTest, sizeof(szTest), "%d", m_nVal);

	sprintf_s(szTest, "%d", m_nVal);

	if (!pRedisAPI->RunCommand(enCmdID_Set, 2, "intk", szTest))
	{
		cout << "  RunCommand Set FAIL  err:" << pRedisAPI->GetLastErrorDesc() << endl;
		return;
	}

	char szString[100] = { 0 };
	TRunCmdResult stResult = { enReply_Status, 0, 100, szString };
	if (!pRedisAPI->GetResult(&stResult, 1))
	{
		cout << "  GetResult return false!  err:" << pRedisAPI->GetLastErrorDesc() << endl;
		return;
	}

	printf("%s %s\n", pRedisAPI->GetLastCmdStr(), stResult.str);


	if (!pRedisAPI->RunCommand(enCmdID_Get, 1, "intk"))
	{
		cout << "  RunCommand Get FAIL  err:" << pRedisAPI->GetLastErrorDesc() << endl;
		return;
	}

	stResult.type = enReply_String;
	if (!pRedisAPI->GetResult(&stResult, 1))
	{
		cout << "  GetResult return false!  err:" << pRedisAPI->GetLastErrorDesc() << endl;
		return;
	}

	printf("%s %s\n", pRedisAPI->GetLastCmdStr(), stResult.str);
}


int CTestTask::GetTaskType()
{
	return 0;
}


LPCSTR CTestTask::GetRunResult()
{
	return "";
}


int CTestTask::GetContextLen()
{
	return 0;
}


bool CTestTask::ExportResult(LPSTR pszContext, int nLen)
{
	return true;
}


void CTestTask::Set(int nVal)
{
	m_nVal = nVal;
}
