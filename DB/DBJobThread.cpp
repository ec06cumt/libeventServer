
#include "..\stdafx.h"
#include "DBJobThread.h"
#include "..\base\packet\JobDBQueueEx.h"
#include "..\base\common\Util.h"
#include "..\Model.h"

extern TCHAR g_szIP[100];
extern TCHAR g_szPort[100];
extern TCHAR g_szDatabase[100];
extern TCHAR g_szUserID[100];
extern TCHAR g_szPWD[100];



CPacketProcManager<LPDBJOB_PROC> g_PacketMgr;

CJobQueueThread* CJobQueueThread::m_pInstance = NULL;

CJobQueueThread::CJobQueueThread()
{

}

CJobQueueThread::~CJobQueueThread()
{

}

bool CJobQueueThread::OnStartup()
{
	SetThreadName("DB_T");
	SetThreadPriority(THREAD_PRIORITY_NORMAL);
	
	SQLWCHAR * pDSNSql = L"DRIVER={SQL Server};SERVER=%s,%s;DATABASE=%s;UID=%s;PWD=%s;";
	SQLWCHAR szDSNFull[200];
	swprintf_s(szDSNFull, 200, pDSNSql, g_szIP, g_szPort,g_szDatabase, g_szUserID, g_szPWD);

	//读取配置文件
	//SQLWCHAR * pDSN = L"DRIVER={SQL Server};SERVER=SH-LIUGAO;DATABASE=uu278;UID=lg2;PWD=lg456&&;";
	//SQLWCHAR * pDSN = L"DRIVER={SQL Server};SERVER=127.0.0.1;DATABASE=uu278;UID=lg2;PWD=lg456&&;";

	bool bOK = m_UserDB.DriverConnectSqlServer(szDSNFull);
	if (bOK)
	{
		GetDBJobQueueEx()->SetUserDB(&m_UserDB);
	}
	else
	{
		m_UserDB.DisconnectSqlServer();
		return false;
	}
	//注册数据库DB函数
	REGISTER_DBJOB_PROC(ACCOUNT_SD_AUTH);
	REGISTER_DBJOB_PROC(ACCOUNT_SD_INFO);

	return true;
}

bool CJobQueueThread::OnCleanup()
{
	m_UserDB.DisconnectSqlServer();

	return true;
}

DWORD WINAPI DBJobQueueThread_Proc_(LPVOID lpData)
{
	CJobQueueThread* pJobQueueThread = (CJobQueueThread*)lpData;

	printf("DBThreadID = %d\n", GetCurrentThreadId());
	return pJobQueueThread->ThreadProc(lpData);
}



DWORD WINAPI CJobQueueThread::ThreadProc(LPVOID lpData)
{
	if (OnStartup())
	{
		while (true)
		{
			if (WAIT_OBJECT_0 == ::WaitForSingleObject(m_hStopEvent, 1)) break;

			GetDBJobQueueEx()->Process();
		}
	}

	GetDBJobQueueEx()->ProcessAll();

	OnCleanup();
	return 0L;
}

void CJobQueueThread::OnReceived(const TCHAR* pBuffer, const DWORD& dwlen)
{
	unsigned short iKey = 0, iLen = 0;
	int iMsgID = 0;
	int ioffset = 0;

	memcpy(&iLen, pBuffer, sizeof(iLen));			ioffset = sizeof(iLen);
	memcpy(&iKey, pBuffer + ioffset, sizeof(iKey)); ioffset += sizeof(iKey);
	memcpy(&iMsgID, pBuffer + ioffset, sizeof(iMsgID)); ioffset += sizeof(iMsgID);


	LPDBJOB_PROC pDBJobProc = g_PacketMgr.FindProc((const UINT)iMsgID);
	if (pDBJobProc)
	{

		pDBJobProc((pBuffer + ioffset),(iLen - ioffset));
	}
}

//bool CJobQueueThread::GetUserInfo(int iUserID, STACCOUNTINFO& stAccountInfo)
//{
//	int iEorrID = 0;
//		
//	m_UserDB.InitBindParam();
//	m_UserDB.BindParam(iEorrID, SQL_PARAM_OUTPUT);
//	m_UserDB.BindParam(iUserID);
//
//	SQLWCHAR * strSql3 = L"{? = call dbo.GSP_GG_AccountInfo_GetInfo(?)}";
//	bool bRet = m_UserDB.ExecuteDirect(strSql3);
//	if (!bRet)
//	{
//		return false;
//	}
//
//	m_UserDB.InitBindCol();
//	m_UserDB.BindCol(stAccountInfo.iUserID);
//	m_UserDB.BindCol(stAccountInfo.szUserName, MAX_NAME_LEN + 1);
//	m_UserDB.BindCol(stAccountInfo.szUserPWD, MAX_NAME_LEN + 1);
//	m_UserDB.BindCol(stAccountInfo.stCreateTime);
//
//	bool bOK = m_UserDB.Fetch();
//	int id = 0;
//	if (bOK)
//	{
//		printf("Account info ID=%d,UserName=%s,UserPassWord=%s,CreateTime:%d-%d-%d %d:%d:%d\n"
//			, stAccountInfo.iUserID, stAccountInfo.szUserName, stAccountInfo.szUserPWD
//			, stAccountInfo.stCreateTime.year, stAccountInfo.stCreateTime.month, stAccountInfo.stCreateTime.day
//			, stAccountInfo.stCreateTime.hour, stAccountInfo.stCreateTime.minute, stAccountInfo.stCreateTime.second);
//	}
//
//	return bOK;
//}

DEFINE_DBJOB_PROC(ACCOUNT_SD_INFO)
{
	UNREFERENCED_PARAMETER(pBuffer);
	UNREFERENCED_PARAMETER(dwlen);

	int iUserID = 0;
	GETVALUE2(iUserID);

	STACCOUNTINFO stAccountInfo;


	CUserDB* pUserDB = GetDBJobQueueEx()->GetUserDB();
	if (!pUserDB) return;

	bool bGet = pUserDB->GetUserInfo(iUserID, stAccountInfo);
	if (bGet)
	{
		printf("DEFINE_DBJOB_PROC(ACCOUNT_SD_INFO)iUserID = %d,UserName=%s,UserPWD=%s\n ", stAccountInfo.iUserID,
			stAccountInfo.szUserName, stAccountInfo.szUserPWD);


		
	}
}

DEFINE_DBJOB_PROC(ACCOUNT_SD_AUTH)
{
	UNREFERENCED_PARAMETER(pBuffer);
	UNREFERENCED_PARAMETER(dwlen);

	char szUserName[MAX_NAME_LEN] = {0};
	char szUserPWD[MAX_PWD_LEN] = { 0 };
	int  iSerectType = 0;
	int  iConnFD = 0;

	GETVALUE2(iConnFD);
	GETDATA2(szUserName, MAX_NAME_LEN);
	GETDATA2(szUserPWD, MAX_NAME_LEN);
	GETVALUE2(iSerectType);

	STACCOUNTINFO stAccountInfo;


	CUserDB* pUserDB = GetDBJobQueueEx()->GetUserDB();
	if (!pUserDB) return;

	bool bGet = pUserDB->GetUserInfoByUserName(szUserName, stAccountInfo);
	if (bGet)
	{
		printf("DEFINE_DBJOB_PROC(ACCOUNT_SD_AUTH)iUserID = %d,UserName=%s,UserPWD=%s\n ", stAccountInfo.iUserID,
			stAccountInfo.szUserName, stAccountInfo.szUserPWD);
		printf("CurrentThreadID = %d\n", GetCurrentThreadId());

		printf("========================================================\n");


		//发消息给客户端
		GOOGLE_PROTOBUF_VERIFY_VERSION;

		Login::Login_SC loginSC;
		std::string strRtCode = "10001";
		loginSC.set_rtcode(strRtCode);
		loginSC.set_uid(stAccountInfo.iUserID);

		std::string str;
		loginSC.SerializeToString(&str);

		CProtoPacket packet(LOGIN_SC_IN);
		packet.PushBack(str.c_str(), (int)str.size());

		GetOnlineConnMgr()->SendPacketToClent(iConnFD, packet.GetData(), packet.GetLen());
	}
	else
	{

	}


}

