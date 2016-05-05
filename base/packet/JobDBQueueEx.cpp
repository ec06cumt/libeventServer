
#include "..\stdafx.h"
#include "JobDBQueueEx.h"
#include "..\DB\DBJobThread.h"

CJobDBQueueEx* CJobDBQueueEx::m_pInstance = NULL;

CJobDBQueueEx::CJobDBQueueEx() :m_uiMsgID(0), m_iWaitHanleMsgCount(0)
{
	CJobDBMsg dbJobMsg;
	bool bChecked = dbJobMsg.CheckMsgSize();
	assert(bChecked);

	if (!bChecked)
	{
		exit(0);
	}
	m_listMsg.clear();
}

CJobDBQueueEx::~CJobDBQueueEx()
{

}

bool CJobDBQueueEx::Init(int nDefaultSize)
{
	m_pCodeQueue = new CCodeQueue(nDefaultSize);
	if (m_pCodeQueue)
	{
		bool bChecked = m_pCodeQueue->CheckIndex();
		assert(bChecked);

		if (!bChecked)
		{
			return false;
		}

		m_uiMsgID = 1;
		return true;
	}
	else
		return false;
}

CJobDBMsg* CJobDBQueueEx::AllocDBJobMsg()
{
	static CJobDBMsg s_cJobDBMsg;
	s_cJobDBMsg.InitEncode();
	return &s_cJobDBMsg;
}

void CJobDBQueueEx::FreeDBJobMsg(CJobDBMsg* pJobDBMsg)
{

}

void CJobDBQueueEx::PushBackJobMsg(CJobDBMsg* pJobMsg)
{
	STDBPACKETINFO stPacketInfo;
	stPacketInfo.m_iPacketLen = pJobMsg->GetRealSize();
	stPacketInfo.m_stPacket = new TCHAR[stPacketInfo.m_iPacketLen];
	//memset(stPacketInfo.m_stPacket, 0, sizeof(unsigned char)*stPacketInfo.m_iPacketLen);
	memcpy(stPacketInfo.m_stPacket, pJobMsg, sizeof(TCHAR)*stPacketInfo.m_iPacketLen);

	m_listMsg.push_back(stPacketInfo);
}

//环形缓冲区，不用加锁，来处理消息队列
bool CJobDBQueueEx::AddDBJobMsg(CJobDBMsg* pJobDBMsg)
{
	assert(pJobDBMsg);
	++m_uiMsgID;
	DWORD dwCreateTime = ::GetTickCount();
	pJobDBMsg->SetMsgID(m_uiMsgID);
	pJobDBMsg->SetCreateTime(dwCreateTime);

	int iAppRet = 0;
	while (m_listMsg.size() > 0)
	{
		STDBPACKETINFO stPacketInfo = m_listMsg.front();
		iAppRet = m_pCodeQueue->AppendOneCode(stPacketInfo.m_stPacket, stPacketInfo.m_iPacketLen);
		if (iAppRet >= 0)
		{
			++m_iWaitHanleMsgCount;
			delete[] stPacketInfo.m_stPacket;
			stPacketInfo.m_stPacket = NULL;
			m_listMsg.pop_front();
		}
		else
		{
			PushBackJobMsg(pJobDBMsg);
			return true;
		}
	}
	//gliu add on 2015/07/23 test start
	int iRealSize = pJobDBMsg->GetRealSize();
	const TCHAR* pInitData = pJobDBMsg->GetData();

	int iLen = *(int*)pInitData;
	int iKey = *(int*)(pInitData+2);
	int iMsgID = *(int*)(pInitData + 4);
	int iInput = *(int*)(pInitData + 8);

	//gliu add on 2015/07/23 test end
	iAppRet = m_pCodeQueue->AppendOneCode((const TCHAR*)pJobDBMsg, pJobDBMsg->GetRealSize());
	if (iAppRet >= 0)
	{
		++m_iWaitHanleMsgCount;
	}
	else
	{
		//处理失败，添加到缓冲队列中，等待处理
		PushBackJobMsg(pJobDBMsg);
	}
	return true;

}

void CJobDBQueueEx::Process()
{
	CJobDBMsg stDBMsg;
	int iMsgLen = 0;
	while (true)
	{
		iMsgLen = sizeof(stDBMsg);

		m_pCodeQueue->GetHeadCode((TCHAR*)&stDBMsg, &iMsgLen);
		
		if (iMsgLen <= 0)
		{
			break;
		}
		printf("Process: pOutLen=%d\n", iMsgLen);

		--m_iWaitHanleMsgCount;

		stDBMsg.SetRealInfo();

		//线程处理
		GetDBJobQueueThread()->OnReceived(stDBMsg.GetData(), stDBMsg.GetLength());
		Sleep(0);
	}
}

void CJobDBQueueEx::ProcessAll()
{
	CJobDBMsg stDBMsg;
	int iMsgLen = 0;
	while (true)
	{
		iMsgLen = sizeof(stDBMsg);
		m_pCodeQueue->GetHeadCode((TCHAR*)&stDBMsg, &iMsgLen);
		stDBMsg.SetRealInfo();
		if (iMsgLen <= 0)
		{
			break;
		}
		--m_iWaitHanleMsgCount;
		
		//线程处理
		GetDBJobQueueThread()->OnReceived(stDBMsg.GetData(), stDBMsg.GetLength());
	}
}