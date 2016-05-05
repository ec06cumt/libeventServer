#include "StdAfx.h"
#include "LibEventFunction.h"

lpFunPtr CLibEventFunction::m_FunPtrList[emMaxPackageType]={0};
int CLibEventFunction::m_stx=0;

CLibEventFunction::CLibEventFunction(void)
{
}

CLibEventFunction::~CLibEventFunction(void)
{
}

void CLibEventFunction::RegistFunc(int id, lpFunPtr func)
{
	m_FunPtrList[id] = func;
}

void CLibEventFunction::RegistConnectedFunc(lpFunPtr func)
{
	m_FunPtrList[emFunConnected]=func;
}

void CLibEventFunction::RegistDisconnectedFunc(lpFunPtr func)
{
	//m_FunPtrList[emFunTimeout] = func;
	//m_FunPtrList[emFunClosed] = func;
	//m_FunPtrList[emFunError] = func;
	m_FunPtrList[emFunDisConnected] = func;
	
}

void CLibEventFunction::RegistTimeoutFunc(lpFunPtr func)
{
	m_FunPtrList[emFunTimeout] = func;
}

void CLibEventFunction::RegistClosedFunc(lpFunPtr func)
{
	m_FunPtrList[emFunClosed] = func;
}

void CLibEventFunction::RegistErrorFunc(lpFunPtr func)
{
	m_FunPtrList[emFunError]=func;
}

void CLibEventFunction::RegistStx(int stx)
{
	m_stx=stx;
}

int CLibEventFunction::LoadStx()
{
	return m_stx;
}

int CLibEventFunction::DispatchFunction(int nFunID,Conn*pItem)
{
	bool bFind = 0;
	if (m_FunPtrList[nFunID])
	{
		bFind = 1;
		if (pItem)
		{
			return m_FunPtrList[nFunID](pItem);
		}		
	}
	return bFind;
}