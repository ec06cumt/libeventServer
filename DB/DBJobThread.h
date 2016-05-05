
#pragma once 
#include "..\base\thread\Thread.h"
#include "..\base\db\UserDB.h"
#include "..\base\common\PacketProcManager.h"
#include "..\PacketID4DB.h"
#include "..\OnlineConnManager.h"

class CJobQueueThread;


typedef void(*LPDBJOB_PROC) (const TCHAR* pBuffer, const DWORD& dwlen);
#define REGISTER_DBJOB_PROC(X) g_PacketMgr.Register(##X,CJobQueueThread::_Proc_##X)
#define DECLARE_DBJOB_PROC(X) static void _Proc_##X(const TCHAR* pBuffer,const DWORD& dwlen)
#define DEFINE_DBJOB_PROC(X) void CJobQueueThread::_Proc_##X(const TCHAR* pBuffer,const DWORD& dwlen)

#define GETVALUE2(X) memcpy(&X,pBuffer,sizeof(X)); pBuffer += sizeof(X)
#define GETDATA2(X,LEN) memcpy(X,pBuffer,LEN); pBuffer += LEN

class CJobQueueThread :public CThread
{
public:
	CJobQueueThread();
	virtual ~CJobQueueThread();

	bool OnStartup();
	bool OnCleanup();

	CUserDB* GetUserDB(){ return &m_UserDB; };

	DWORD WINAPI ThreadProc(LPVOID lpData);
	static void OnReceived(const TCHAR* pBuffer, const DWORD& dwlen);


	static CJobQueueThread* GetInstance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CJobQueueThread();
		}
		return m_pInstance;
	}

	//ÉùÃ÷DBº¯Êý
	DECLARE_DBJOB_PROC(ACCOUNT_SD_AUTH);
	DECLARE_DBJOB_PROC(ACCOUNT_SD_INFO);

private:
	static CJobQueueThread* m_pInstance;

	CJobQueueThread(CJobQueueThread& jobQueueThread);

	CUserDB m_UserDB;
private:
	//bool GetUserInfo(int iUserID, STACCOUNTINFO& stAccountInfo);

};

#define GetDBJobQueueThread() (CJobQueueThread::GetInstance())

extern DWORD WINAPI DBJobQueueThread_Proc_(LPVOID lpData);