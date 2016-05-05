
#pragma once
#include <hash_map>

#include "../base/lock/LockObject.h"

class CSessionContext
{
public :
	CSessionContext();
	~CSessionContext();
	int GetSessionID(){ return m_iSessionID; };
	
private :
	int m_iSessionID;
	CLockObject lockObject;


};