
#include "..\stdafx.h"
#include "PacketForDB.h"

CPacketForDB::CPacketForDB()
{

}

CPacketForDB::~CPacketForDB()
{

}

void CPacketForDB::InitPacketDB()
{
	m_iLen = 0;
	m_iKey = 0;
	m_bHasData = false;
}

void CPacketForDB::Init(const unsigned int iPacketID)
{
	InitPacketDB();
	m_pBuffer = m_pData;

	m_iLen = 2 + 2 + 4;
	memcpy(m_pBuffer, &m_iLen, sizeof(unsigned short));
	m_pBuffer += sizeof(unsigned short);
	memcpy(m_pBuffer, &m_iKey, sizeof(unsigned short));
	m_pBuffer += sizeof(unsigned short);

	memcpy(m_pBuffer, &iPacketID, sizeof(unsigned int));
	m_pBuffer += sizeof(unsigned int);

	m_bHasData = true;
}


void CPacketForDB::SetKey(unsigned short usKey)
{
	m_iKey = usKey;

	m_pData += sizeof(unsigned short);
	memcpy(m_pData, &usKey, sizeof(unsigned int));

}

void CPacketForDB::PushBack(const LPVOID pData, const unsigned short iLen)
{
	if (MAX_PACKETDB_SIZE < m_iLen + iLen)
	{
		assert(false && "MAX_PACKETDB_SIZE < m_iLen + iLen");
		return;
	}

	memcpy(m_pBuffer, pData, iLen);	m_pBuffer += iLen;
	m_iLen += iLen;
	memcpy(m_pData, &m_iLen, sizeof(unsigned short));
}

void CPacketForDB::PushBack(const TCHAR* pData, const unsigned short iLen)
{
	if (MAX_PACKETDB_SIZE < m_iLen + iLen)
	{
		assert(false && "MAX_PACKETDB_SIZE < m_iLen + iLen");
		return;
	}

	memcpy(m_pBuffer, pData, iLen);	m_pBuffer += iLen;
	m_iLen += iLen;
	memcpy(m_pData, &m_iLen, sizeof(unsigned short));
}

void CPacketForDB::PushBack(const TCHAR value)
{
	PushBack(&value, sizeof(value));
}

void CPacketForDB::PushBack(const short value)
{
	PushBack((const TCHAR*)&value, sizeof(value));
}

void CPacketForDB::PushBack(const int value)
{
	PushBack((const TCHAR*)&value, sizeof(value));
}

void CPacketForDB::PushBack(const long value)
{
	PushBack((const TCHAR*)(const TCHAR*)&value, sizeof(value));
}

void CPacketForDB::PushBack(const float value)
{
	PushBack((const TCHAR*)(const TCHAR*)&value, sizeof(value));
}

void CPacketForDB::PushBack(const double value)
{
	PushBack((const TCHAR*)&value, sizeof(value));
}

void CPacketForDB::PushBack(const unsigned char value)
{
	PushBack((const TCHAR*)&value, sizeof(value));
}

void CPacketForDB::PushBack(const unsigned short value)
{
	PushBack((const TCHAR*)&value, sizeof(value));
}

void CPacketForDB::PushBack(const unsigned int value)
{
	PushBack((const TCHAR*)&value, sizeof(value));
}

void CPacketForDB::PushBack(const unsigned long value)
{
	PushBack((const TCHAR*)&value, sizeof(value));
}

void CPacketForDB::PushBack(const SYSTEMTIME timeDate)
{
	PushBack((const TCHAR*)&timeDate, sizeof(timeDate));
}

void CPacketForDB::PushBack(const INT64 value)
{
	PushBack((const TCHAR*)&value, sizeof(value));
}

void CPacketForDB::PushBack(const UINT64 value)
{
	PushBack((const TCHAR*)&value, sizeof(value));
}


