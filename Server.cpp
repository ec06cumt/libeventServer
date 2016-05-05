// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "LibEvent.h"
#include "base\db\DBSqlServer.h"
//#include "base\db\DBMySql.h"
#include "base\common\Util.h"
#include "base\packet\JobDBQueueEx.h"
#include "DB\DBJobThread.h"

#include "..\Model.h"

#include <memory>


extern int g_iClientNum;

SQLWCHAR* CharToWchar(const char* c)
{
	SQLWCHAR *m_wchar = NULL;
	int len = MultiByteToWideChar(CP_ACP, 0, c, strlen(c), NULL, 0);
	m_wchar = new SQLWCHAR[len + 1];
	MultiByteToWideChar(CP_ACP, 0, c, strlen(c), m_wchar, len);
	m_wchar[len] = '\0';
	return m_wchar;
}



int _tmain(int argc, _TCHAR* argv[])
{	
	//protobuf ��֤
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	///��ȡ�����ļ�
	bool bRet = CUtilHelper::LoadDBConf();
	if (!bRet)
		return 0;

	//��ʼ�����ݿ���л��λ������Ĵ�С
	int iDefalutSize = (g_iClientNum/10) * 1024 * 1024;
	if (!GetDBJobQueueEx()->Init(iDefalutSize))
	{
		return 0;
	}
	//��ʼ�����ݿ��߳�
	if (!GetDBJobQueueThread()->Create(DBJobQueueThread_Proc_, (LPVOID)GetDBJobQueueThread()))
	{
		return 0;
	}
	GetDBJobQueueThread()->Run();


	CLibEvent libEvent;
	libEvent.StartServer(7000,2,1000,60*2,60*2);
	char c = getchar();
	if (c == 'c')
	{
		libEvent.StopServer();
	}
	
	return 0;
}

