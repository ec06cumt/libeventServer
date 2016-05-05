
#pragma once 
#include "DBSqlServer.h"
#include "..\PacketID4DB.h"

class CUserDB :public CDBSqlServer
{
public:
	CUserDB();
	virtual ~CUserDB();

	bool GetUserInfo(int iUserID, STACCOUNTINFO& stAccountInfo);
	bool GetUserInfo2(int iUserID);
	bool GetUserInfoByUserName(char* pUserName, STACCOUNTINFO& stAccountInfo);

};