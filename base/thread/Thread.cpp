
#include "..\stdafx.h"
#include "Thread.h"
#include <process.h>

CThread::CThread() :m_hThread(NULL), m_dwThreadID(0), m_hStopEvent(NULL)
{
	memset(m_szThreadName,0,sizeof(char)*255);
}

CThread::~CThread()
{
	CloseHandle(m_hThread);
	CloseHandle(m_hStopEvent);
}

DWORD CThread::GetThreadID() const
{
	return m_dwThreadID;
}

HANDLE CThread::GetThreadHandle()
{
	return m_hThread;
}

void CThread::SetThreadPriority(DWORD dwPriority)
{
	::SetThreadPriority(m_hThread, dwPriority);

}

void CThread::SetThreadName(char* szName)
{
	if (szName)
	{
		int iName = strlen(szName)>255 ? 255 : strlen(szName);
		memcpy(m_szThreadName, szName, iName);
	}
}

char* CThread::GetThreadName()
{
	return m_szThreadName;
}

bool CThread::Create(THREADPROC threadProc, LPVOID lpData)
{
	if (!threadProc) return false;

	m_hStopEvent = ::CreateEvent(NULL,false,false,NULL);
	if (!m_hStopEvent)
	{
		return false;
	}

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, (unsigned int(_stdcall *)(void*))threadProc, lpData,CREATE_SUSPENDED,(unsigned int*)&m_dwThreadID);
	//m_hThread = CreateThread(NULL, 0, threadProc, lpData, 0, (LPDWORD)&m_dwThreadID);
	if (m_hThread == NULL)
	{
		CloseHandle(m_hStopEvent);
		return false;
	}
	return true;
}

void CThread::Stop()
{
	::SetEvent(m_hStopEvent);
}

void CThread::Pause() const
{
	::SuspendThread(m_hThread);
}

void CThread::Run() const
{
	::ResumeThread(m_hThread);
}

bool CThread::WaitForStop(DWORD dwTimeOut) const
{
	if (WAIT_OBJECT_0 == ::WaitForSingleObject(m_hThread, dwTimeOut))
	{
		return true;
	}

	return false;
}

