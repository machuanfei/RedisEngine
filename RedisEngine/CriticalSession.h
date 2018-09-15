// CriticalSession.h: interface for the CCriticalSession class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRITICALSESSION_H__0FBB637B_1ABE_4359_8E47_F22695EE828D__INCLUDED_)
#define AFX_CRITICALSESSION_H__0FBB637B_1ABE_4359_8E47_F22695EE828D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <windows.h>



class CCriticalSession  
{
private:
	CRITICAL_SECTION m_sect;


public: 
	CCriticalSession()
	{
		InitializeCriticalSection(&m_sect);
	}

	CCriticalSession(const CCriticalSession &other)
	{
		InitializeCriticalSection(&m_sect);
	}

	~CCriticalSession()
	{
		DeleteCriticalSection(&m_sect);
	}

	inline void Lock()
	{	
		EnterCriticalSection(&m_sect);
	}
	inline void Unlock()
	{		
		LeaveCriticalSection(&m_sect);
	}

	//BOOL TryEnter();
};

#endif // !defined(AFX_CRITICALSESSION_H__0FBB637B_1ABE_4359_8E47_F22695EE828D__INCLUDED_)


