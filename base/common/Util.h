
#pragma once
#include <Windows.h>

class CUtilHelper
{
public:
	CUtilHelper();
	virtual ~CUtilHelper();

	static TCHAR* GetCurrDir(TCHAR* pPath);
	static bool LoadDBConf();

	static bool ConvertTCHAR2CHAR(TCHAR* pWChar, int iSrcLen, char* pChar, int iDesLen);
	static bool ConvertCHAR2TCHAR(char* pSrcChar, int SrcLen, TCHAR* pWChar, int iDesLen);
public:

};