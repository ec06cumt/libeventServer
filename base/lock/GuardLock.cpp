
#include "..\stdafx.h"
#include "GuardLock.h"

CGuardLock::CGuardLock(CLockObject* lockObject):m_lockObject(lockObject)
{
	if (lockObject)
		lockObject->Lock();
}

CGuardLock::~CGuardLock()
{
	if (m_lockObject)
	{
		m_lockObject->Unlock();
	}
}