
#pragma once 
#include <Windows.h>

typedef DWORD(WINAPI *THREADPROC)(LPVOID lpData);

class CThread
{
public:

	CThread();
	virtual ~CThread();

	virtual bool Create(THREADPROC threadProc, LPVOID lpData);
	virtual void Stop();
	virtual bool OnStartup() = 0;
	virtual bool OnCleanup() = 0;

	bool WaitForStop(DWORD dwTimeOut) const;
	void Pause() const;
	void Run() const;

	DWORD WINAPI ThreadProc(LPVOID lpData);

	DWORD GetThreadID() const;
	HANDLE GetThreadHandle();
	void SetThreadPriority(DWORD dwPriority);
	void SetThreadName(char* szName);
	char* GetThreadName();
protected:
	HANDLE m_hStopEvent;

private:
	DWORD m_dwThreadID;
	HANDLE m_hThread;
	char m_szThreadName[255];
};