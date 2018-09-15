#pragma once


class CTestTask : public IRedisTask
{
public:
	CTestTask();
	~CTestTask();

	// 执行
	virtual void Do(IRedisAPI* pRedisAPI);

	// 获取任务类型
	virtual int GetTaskType();

	// 获取执行结果
	virtual LPCSTR GetRunResult();

	// 获取现场大小
	virtual int GetContextLen();

	// 导出结果
	virtual bool ExportResult(LPSTR pszContext, int nLen);

public:
	// 设置参数
	void Set(int nVal);

private:
	int m_nVal;
};

