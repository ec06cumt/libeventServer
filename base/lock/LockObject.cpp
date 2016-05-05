
#include "..\stdafx.h"
#include "LockObject.h"

CLockObject::CLockObject()
{
	::InitializeCriticalSection(&m_Cs);
}

CLockObject::~CLockObject()
{
	::DeleteCriticalSection(&m_Cs);
}

void CLockObject::Lock()
{
	::EnterCriticalSection(&m_Cs);
}

void CLockObject::Unlock()
{
	::LeaveCriticalSection(&m_Cs);
}