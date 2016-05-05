
#pragma once 
#include "codequeue.h"
#include "JobDBMsg.h"
#include "..\lock\LockObject.h"
#include "..\db\UserDB.h"
#include <list>

typedef struct STDBPACKINFO
{
	TCHAR* m_stPacket;
	int m_iPacketLen;
}STDBPACKETINFO,*PDBPACKETINFO;

class CJobDBQueueEx
{
public:
	
	bool Init(int nDefaultSize);

	void Process();

	void ProcessAll();

	CJobDBMsg* AllocDBJobMsg();
	void FreeDBJobMsg(CJobDBMsg* pJobDBMsg);

	bool AddDBJobMsg(CJobDBMsg* pJobDBMsg);

	int GetWaitHandleMsgCount(){ return m_iWaitHanleMsgCount; };
	int GetDBListSize(){ return m_listMsg.size(); };

	int GetDBQueueSize() { return m_queueMsg.size(); };
	int GetDBDirtyQueueSize() { return m_queueDirtyMsg.size(); };

	int GetQueueUsedSize();


	CUserDB* GetUserDB(){ return m_UserDB; };
	void SetUserDB(CUserDB* pUserDB) { m_UserDB = pUserDB; };

	static CJobDBQueueEx* GetInstance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CJobDBQueueEx();
		}
		return m_pInstance;
	}

private:

	UINT32 m_uiMsgID;				//处理消息的ID，累加所得
	int		m_iWaitHanleMsgCount;
	CLockObject m_lockObj;
	CCodeQueue* m_pCodeQueue;

	CJobDBQueueEx();
	virtual ~CJobDBQueueEx();

	CUserDB* m_UserDB;
	std::list<STDBPACKETINFO> m_listMsg;

	std::queue<CJobDBMsg*> m_queueDirtyMsg;

	std::queue<CJobDBMsg*> m_queueMsg;

	static CJobDBQueueEx* m_pInstance;

	CJobDBQueueEx(const CJobDBQueueEx& jobDBQueue);
	CJobDBQueueEx& operator =(const CJobDBQueueEx&);

private:
	void PushBackJobMsg(CJobDBMsg* pJobMsg);
};

#define GetDBJobQueueEx() (CJobDBQueueEx::GetInstance())