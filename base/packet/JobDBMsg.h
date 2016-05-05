
#pragma once 
#include <Windows.h>
#include <assert.h>
#include "PacketForDB.h"

#define MAX_PACKETDB_SIZE 8192

class CJobDBMsg:public CPacketForDB
{
public:
	CJobDBMsg();
	~CJobDBMsg();

	void Init(const unsigned int uiPacketID);

	void InitEncode()
	{
		m_pData = m_pDataReal;
		InitPacketDB();
	}

	void SetMsgID(UINT32 uiMsgID)
	{
		m_uiMsgID = uiMsgID;
	}

	UINT32 GetMsgID()
	{
		return m_uiMsgID;
	}

	void SetCreateTime(DWORD dw)
	{
		m_stCreateTime = dw;
	}

	UINT32 GetCreateTime()
	{
		return m_stCreateTime;
	}

	int GetRealSize()
	{
		int iSize =  sizeof(*this) - sizeof(m_pDataReal) + m_iLen;
		return iSize;
	}

	bool CheckMsgSize()
	{
		ULONG iSize = sizeof(*this);

		ULONG ioffset = ULONG((ULONG)(this->m_pDataReal) - (ULONG)(this));
		if (iSize - ioffset == sizeof(this->m_pDataReal))
		{
			return true;
		}
		return false;
	}

	void SetRealInfo()
	{
		m_pBuffer = m_pDataReal;
		m_pData = m_pDataReal;
	}

private:
	UINT32 m_uiMsgID;
	DWORD  m_stCreateTime;

	TCHAR m_pDataReal[MAX_PACKETDB_SIZE];


};