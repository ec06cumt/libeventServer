#include "StdAfx.h"
#include "LibUserFunction.h"
#include "LibEvent.h"
#include "base\packet\JobDBQueueEx.h"

#include "..\Model.h"

CLibUserFunction::CLibUserFunction(void)
{
}

CLibUserFunction::~CLibUserFunction(void)
{
}

int CLibUserFunction::Connect(Conn* c)
{
	static int iConnectNum = 0;
	iConnectNum++;
	printf("CLibUserFunction::Connect =%d\n", iConnectNum);
	return 0;
}

int CLibUserFunction::DisConnect(Conn* c)
{
	printf("CLibUserFunction::Disconnect: %d\n", c->fd);
	return 0;
}

int CLibUserFunction::TimeOut(Conn* c)
{
	printf("CLibUserFunction::TimeOut: %d\n", c->fd);
	return 0;
}

int CLibUserFunction::Login(Conn* c)
{

	CPacketHead *hreq = (CPacketHead*)c->in_buf;

	int PersonStrLen = c->in_buf_len - sizeof(CPacketHead);

	char szBuffer[8196] = {0};

	memcpy(szBuffer, c->in_buf + sizeof(CPacketHead), PersonStrLen);
	std::string strBuffer = szBuffer;

	
	Login::Login_CS loginCS;
	loginCS.ParseFromString(strBuffer);

	printf("Client Login: fd=%d,username=%s,PWD=%s,type=%d\n", c->fd, loginCS.username().c_str(), loginCS.userpassword().c_str()
		, loginCS.isercettype());


	//操作数据库验证
	CJobDBMsg* pJobMsg = GetDBJobQueueEx()->AllocDBJobMsg();
	if (!pJobMsg) 
		return 0;
	int iSerectType = loginCS.isercettype();
	char szUserName[MAX_NAME_LEN] = {0};
	char szUserPWD[MAX_PWD_LEN] = { 0 };
	int iLen = loginCS.username().size() > MAX_NAME_LEN ? MAX_NAME_LEN : loginCS.username().size();
	memcpy(szUserName, loginCS.username().c_str(), iLen);

	iLen = loginCS.userpassword().size() > MAX_PWD_LEN ? MAX_PWD_LEN : loginCS.userpassword().size();
	memcpy(szUserPWD, loginCS.userpassword().c_str(), iLen);

	int iConnectFD = c->fd;			//服务器唯一标识
	bool bAdd = false;

	pJobMsg->Init(ACCOUNT_SD_AUTH);
	pJobMsg->PushBack(iConnectFD);
	pJobMsg->PushBack(szUserName, MAX_NAME_LEN);
	pJobMsg->PushBack(szUserPWD, MAX_PWD_LEN);
	pJobMsg->PushBack(iSerectType);
	bAdd = GetDBJobQueueEx()->AddDBJobMsg(pJobMsg);
	
	if (!bAdd)
	{
		GetDBJobQueueEx()->FreeDBJobMsg(pJobMsg);
	}
	
	return LOGIN_SC_IN;
}


int CLibUserFunction::Logout(Conn* c)
{
	printf("Logout: %d\n", c->fd);
	return CONNECT_SC_CLOSE;
}
