
#pragma once 

#define MAX_NAME_LEN 25
#define MAX_PWD_LEN 25

enum
{
	ENUM_NULL,
	ACCOUNT_SD_AUTH,
	ACCOUNT_SD_INFO,
	//int iUserID		//ÓÃ»§ID

	ENUM_MAX
};

#pragma pack(push, 1)

typedef struct AccountInfo
{
	char szUserName[MAX_NAME_LEN + 1];
	char szUserPWD[MAX_PWD_LEN + 1];
	int iUserID;
	TIMESTAMP_STRUCT stCreateTime;
	AccountInfo()
	{
		iUserID = 0;
		memset(szUserName, 0, sizeof(char)*(MAX_NAME_LEN + 1));
		memset(szUserPWD, 0, sizeof(char)*(MAX_PWD_LEN + 1));
	}

}STACCOUNTINFO, *PSTACCOUNTINFO;

#pragma pack(pop)