
#include "..\stdafx.h"
#include "Util.h"

TCHAR g_szIP[100] = TEXT("");
TCHAR g_szPort[100] = TEXT("");
TCHAR g_szDatabase[100] = TEXT("");
TCHAR g_szUserID[100] = TEXT("");
TCHAR g_szPWD[100] = TEXT("");

int   g_iClientNum = 0;

CUtilHelper::CUtilHelper()
{
}

CUtilHelper::~CUtilHelper()
{

}

TCHAR* CUtilHelper::GetCurrDir(TCHAR* pPath)
{
	//TCHAR pPath[MAX_PATH] = {0};
	DWORD dwLen = ::GetModuleFileName(NULL, pPath, MAX_PATH + 1);
	if (dwLen == 0)
	{
		return NULL;
	}

	int iLenSize = dwLen - 1;

	while (true)
	{
		if (TEXT('\\') == pPath[iLenSize])
		{
			pPath[iLenSize+1] = TEXT('\0');
			return pPath;
		}
		if (iLenSize <= 0)
		{
			return NULL;
		}
		iLenSize--;
	}


}

bool CUtilHelper::LoadDBConf()
{
	TCHAR pPathDir[MAX_PATH] = {0};

	TCHAR* pGet = GetCurrDir(pPathDir);
	if (!pGet)
		return false;

	TCHAR pPathDBDir[MAX_PATH] = { 0 };
	_tcscpy(pPathDBDir, pPathDir);
	//DB数据库配置文件
	TCHAR* pDBINI = TEXT("config\\MssqlDB.ini");
	_tcscat(pPathDBDir, pDBINI);
	TCHAR* pSectionDB = TEXT("AccountDB");

	GetPrivateProfileString(pSectionDB, TEXT("IP"), TEXT(""), g_szIP, 100, pPathDBDir);
	GetPrivateProfileString(pSectionDB, TEXT("Port"), TEXT(""), g_szPort, 100, pPathDBDir);
	GetPrivateProfileString(pSectionDB, TEXT("DATABASE"), TEXT(""), g_szDatabase, 100, pPathDBDir);
	GetPrivateProfileString(pSectionDB, TEXT("USERID"), TEXT(""), g_szUserID, 100, pPathDBDir);
	GetPrivateProfileString(pSectionDB, TEXT("PWD"), TEXT(""), g_szPWD, 100, pPathDBDir);

	TCHAR* pSectionClient = TEXT("Client");
	g_iClientNum = GetPrivateProfileInt(pSectionClient, TEXT("ClientNum"), 0, pPathDBDir);

	return true;
}

bool CUtilHelper::ConvertTCHAR2CHAR(TCHAR* pWChar,int iSrcLen, char* pChar,int iDesLen)
{
	if (iSrcLen == 0 || !pWChar || !pChar)
	{
		return false;
	}
	//LPSTR pszOut = NULL;
	
	//int nInputStrLen = wcslen(pWChar);

	// Double NULL Termination  
	int nOutputStrLen = WideCharToMultiByte(CP_ACP, 0, pWChar, iSrcLen, NULL, 0, 0, 0) + 2;
	//pszOut = new char[nOutputStrLen];
	if (iDesLen < nOutputStrLen)
	{
		return false;
	}
	if (pChar)
	{
		memset(pChar, 0x00, nOutputStrLen);
		WideCharToMultiByte(CP_ACP, 0, pWChar, iSrcLen, pChar, nOutputStrLen, 0, 0);
	}

	return true;
}

bool CUtilHelper::ConvertCHAR2TCHAR(char* pSrcChar, int SrcLen, TCHAR* pWChar, int iDesLen)
{
	if (!pWChar || !pSrcChar || (SrcLen == 0) || (iDesLen == 0))
	{
		return false;
	}
#ifdef UNICODE  
	MultiByteToWideChar(CP_ACP, 0, pSrcChar, -1, pWChar, iDesLen);
#else  
	strcpy(pWChar, pSrcChar);
#endif  
	return true;
}