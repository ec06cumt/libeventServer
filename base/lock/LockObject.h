
#pragma once

#include<windows.h>

class CLockObject
{
public:
	CLockObject();
	~CLockObject();
	void Lock();
	void Unlock();
public:
	friend class CGuardLock;
private:
	CRITICAL_SECTION  m_Cs;
};