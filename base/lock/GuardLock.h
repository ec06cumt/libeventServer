
#pragma once 
#include "LockObject.h"

class CGuardLock
{
public:
	CGuardLock(CLockObject* lockObject);
	~CGuardLock();
private:
	CLockObject* m_lockObject;
};