#pragma once


class CTestTask : public IRedisTask
{
public:
	CTestTask();
	~CTestTask();

	// ִ��
	virtual void Do(IRedisAPI* pRedisAPI);

	// ��ȡ��������
	virtual int GetTaskType();

	// ��ȡִ�н��
	virtual LPCSTR GetRunResult();

	// ��ȡ�ֳ���С
	virtual int GetContextLen();

	// �������
	virtual bool ExportResult(LPSTR pszContext, int nLen);

public:
	// ���ò���
	void Set(int nVal);

private:
	int m_nVal;
};

