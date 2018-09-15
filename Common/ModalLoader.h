/********************************************************************
	created:	2011/05/23
	created:	23:5:2011   1:24
	filename: 	E:\ws\VC2003\CardSrv\common\ModalLoader.h
	file path:	E:\ws\VC2003\CardSrv\common
	file base:	ModalLoader
	file ext:	h
	author:		
	
	purpose:	模块加载器
*********************************************************************/


#ifndef __MODALLOADER_H__
#define __MODALLOADER_H__

typedef void* (*TGET_FN)(LPCSTR pszInterfaceName);

#include <map>
#include <string>	

// #ifdef  _UNICODE
// #define _tstring wstring
// #else
// #define  _tstring string
// #endif



#if (defined(_WIN32) || defined(_WIN64))


class CModalLoader
{
private:
	HMODULE m_hDll;
	TGET_FN m_pFN;


public:

	CModalLoader()
	{
		m_hDll = NULL;
		m_pFN = NULL;
	}

	~CModalLoader()
	{
		FreeModal();
	}

	void FreeModal()
	{
		if (m_hDll != NULL)
		{
			FreeLibrary(m_hDll);
			m_hDll = NULL;
		}
	}

	bool LoadModal(LPCSTR pszFileName)
	{
		if (m_hDll != NULL)
		{
			return true;
		}

		m_hDll = LoadLibraryA(pszFileName);
		if (!m_hDll)
		{
			return false;
		}

		m_pFN = (TGET_FN)GetProcAddress(m_hDll, "GetModuleInterface");
		return m_pFN != NULL;
	}

	char *GetErrorMsg()
	{
		static char szErrorMsg[256];
		sprintf_s(szErrorMsg, "error code:%d", GetLastError());
		return szErrorMsg;
	}

	//   加载接口	
	void *GetInterface(LPCSTR pszInterfaceName)
	{
		if (!pszInterfaceName || !m_pFN)
		{
			return NULL;
		}

		return m_pFN(pszInterfaceName);
	}
};




#else

//  linux 下实现
#include <dlfcn.h>
class CModalLoader
{
private:
	void* m_hDll;
	TGET_FN m_pFN;


public:

	CModalLoader()
	{
		m_hDll = NULL;
		m_pFN = NULL;
	}

	~CModalLoader()
	{
		FreeModal();
	}

	void FreeModal()
	{
		if (m_hDll != NULL)
		{
			dlclose(m_hDll);
			m_hDll = NULL;
		}
	}

	bool LoadModal(const char *pszFileName)
	{
		m_hDll = dlopen(pszFileName, RTLD_LAZY);
		if (!m_hDll)
		{			
			return false;
		}

		m_pFN = (TGET_FN)dlsym(m_hDll, "GetModuleInterface");
		return m_pFN != NULL;
	}

	char *GetErrorMsg()
	{
		return  dlerror();
	}


	//   加载接口	
	void *GetInterface(const char *pszInterfaceName)
	{
		if (!pszInterfaceName || !m_pFN)
		{
			return NULL;
		}

		return m_pFN(pszInterfaceName);
	}
};





#endif



class CInterfaceLoader
{
private:
	typedef map<string, CModalLoader *>  TModuleMap;

private:
	TModuleMap  m_oModuleMap;
	string   m_strLastError;

public:
	CInterfaceLoader(void) {};
	~CInterfaceLoader(void)
	{
		for (TModuleMap::iterator itModule = m_oModuleMap.begin(); itModule != m_oModuleMap.end(); ++itModule)
		{
			delete (*itModule).second;
		}
	}

	LPCSTR GetLastError()
	{
		return m_strLastError.c_str();
	}
	
	
	void *CreateInterface(LPCSTR szModuleName, LPCSTR szInterfaceName)
	{
		//  获取/新建加载器
		CModalLoader *pModalLoader = m_oModuleMap[szModuleName];
		if (pModalLoader == NULL)
		{
			pModalLoader = new CModalLoader;
			m_oModuleMap[szModuleName] = pModalLoader;
		}

		if (!pModalLoader->LoadModal(szModuleName))
		{
			m_strLastError = string(szModuleName) + string(" load fail.");
			return false;
		}

		void *pRetInterface = pModalLoader->GetInterface(szInterfaceName);
		if (pRetInterface == NULL)
		{
			m_strLastError = string("Create Interface fail,   Name:") + string(szInterfaceName);
			return false;
		}

		return pRetInterface;
	}

};





#endif