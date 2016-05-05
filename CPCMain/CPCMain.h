
#pragma once
#include <Windows.h>
#include "..\DB\DBJobThread.h"


class CPCMain
{
public:
	CPCMain();
	~CPCMain();

	static void Send_SD_GetAccountInfo(int iUserID,STACCOUNTINFO& stAccountInfo);

};