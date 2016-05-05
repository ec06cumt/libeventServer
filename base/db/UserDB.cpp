
#include "..\stdafx.h"
#include "UserDB.h"

CUserDB::CUserDB()
{

}

CUserDB::~CUserDB()
{

}

bool CUserDB::GetUserInfo(int iUserID, STACCOUNTINFO& stAccountInfo)
{
	int iEorrID = 0;
	//test
	if (iUserID <= 0)
	{
		return false;
	}

	InitBindParam();
	BindParam(iEorrID, SQL_PARAM_OUTPUT);
	BindParam(iUserID);

	SQLWCHAR * strSql3 = L"{? = call dbo.GSP_GG_AccountInfo_GetInfo(?)}";
	bool bRet =ExecuteDirect(strSql3);
	if (!bRet)
	{
		return false;
	}

	InitBindCol();
	BindCol(stAccountInfo.iUserID);
	BindCol(stAccountInfo.szUserName, MAX_NAME_LEN + 1);
	BindCol(stAccountInfo.szUserPWD, MAX_PWD_LEN + 1);
	BindCol(stAccountInfo.stCreateTime);

	bool bOK = Fetch();
	int id = 0;
	if (bOK)
	{
		printf("Account info ID=%d,UserName=%s,UserPassWord=%s,CreateTime:%d-%d-%d %d:%d:%d\n"
			, stAccountInfo.iUserID, stAccountInfo.szUserName, stAccountInfo.szUserPWD
			, stAccountInfo.stCreateTime.year, stAccountInfo.stCreateTime.month, stAccountInfo.stCreateTime.day
			, stAccountInfo.stCreateTime.hour, stAccountInfo.stCreateTime.minute, stAccountInfo.stCreateTime.second);
	}
	return bOK;
}

bool CUserDB::GetUserInfo2(int iUserID)
{
	int iEorrID = 0;
	//test
	if (iUserID <= 0)
	{
		return false;
	}

	InitBindParam();
	BindParam(iEorrID, SQL_PARAM_OUTPUT);
	BindParam(iUserID);

	SQLWCHAR * strSql3 = L"{? = call dbo.GSP_GG_AccountInfo_GetInfo(?)}";
	bool bRet = ExecuteDirect(strSql3);
	if (!bRet)
	{
		return false;
	}
	int iOutUserID = 0;
	char szUserName[MAX_NAME_LEN+1];
	char szUserPWD[MAX_NAME_LEN + 1];
	TIMESTAMP_STRUCT stCreateTime;

	InitBindCol();
	BindCol(iOutUserID);
	BindCol(szUserName, MAX_NAME_LEN+1);
	BindCol(szUserPWD, MAX_NAME_LEN+1);
	BindCol(stCreateTime);

	bool bOK = Fetch();
	int id = 0;
	if (bOK)
	{
		printf("Account info ID=%d,UserName=%s,UserPassWord=%s,CreateTime:%d-%d-%d %d:%d:%d\n"
			, iUserID, szUserName, szUserPWD
			, stCreateTime.year, stCreateTime.month, stCreateTime.day
			, stCreateTime.hour, stCreateTime.minute, stCreateTime.second);
	}
	return bOK;
}

bool CUserDB::GetUserInfoByUserName(char* pUserName, STACCOUNTINFO& stAccountInfo)
{
	int iEorrID = 0;

	//test
	if (!pUserName)
	{
		return false;
	}

	InitBindParam();
	BindParam(iEorrID, SQL_PARAM_OUTPUT);
	BindParam(pUserName,MAX_NAME_LEN);

	SQLWCHAR * strSql3 = L"{? = call dbo.GSP_GG_AccountInfo_GetInfoByUserName(?)}";
	bool bRet = ExecuteDirect(strSql3);
	if (!bRet)
	{
		return false;
	}
	int iOutUserID = 0;
	char szUserName[MAX_NAME_LEN + 1] = {0};
	char szUserPWD[MAX_NAME_LEN + 1] = {0};
	TIMESTAMP_STRUCT stCreateTime;

	InitBindCol();
	BindCol(stAccountInfo.iUserID);
	BindCol(stAccountInfo.szUserName, MAX_NAME_LEN);
	BindCol(stAccountInfo.szUserPWD, MAX_NAME_LEN);
	BindCol(stAccountInfo.stCreateTime);
	stCreateTime = stAccountInfo.stCreateTime;

	bool bOK = Fetch();
	if (bOK)
	{

		printf("Account info ID=%d,UserName=%s,UserPassWord=%s,CreateTime:%d-%d-%d %d:%d:%d\n"
			, stAccountInfo.iUserID, stAccountInfo.szUserName, stAccountInfo.szUserPWD
			, stCreateTime.year, stCreateTime.month, stCreateTime.day
			, stCreateTime.hour, stCreateTime.minute, stCreateTime.second);
	}
	return bOK;
}