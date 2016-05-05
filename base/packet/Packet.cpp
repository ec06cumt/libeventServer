
#include "..\stdafx.h"
#include "Packet.h"

CPacket::CPacket() :m_tRenctRevPacketTime(0), m_iDataLen(0), m_iPointPos(0)
{
	memset(m_pData, 0, sizeof(MAX_PACKET_LEN));
	m_iPointPos = 0;
	CPacketHead packetHeader;
	//初始化包头位置
	memcpy(m_pData, (char*)(&(packetHeader.iKey)), sizeof(int));
	m_iDataLen += sizeof(int);
	memcpy(m_pData + m_iDataLen, (char*)(&(packetHeader.pkglen)), sizeof(int));
	m_iDataLen += sizeof(int);

	memcpy(m_pData + m_iDataLen, (char*)(&(packetHeader.iPacketID)), sizeof(int));
	m_iDataLen += sizeof(int);
	//
	m_tRenctRevPacketTime = 0;
	m_tCreateTime = 0;
	m_bSetHeader = false;
}

CPacket::~CPacket()
{

}

CPacket::CPacket(CPacketHead& packetHeader)
{
	m_stPacketHeader = packetHeader;
	m_bSetHeader = true;
	m_iDataLen = 0;

	memset(m_pData, 0, sizeof(MAX_PACKET_LEN));

	memcpy(m_pData, (char*)(&(packetHeader.iKey)), sizeof(int));
	m_iDataLen += sizeof(int);
	memcpy(m_pData + m_iDataLen, (char*)(&(packetHeader.pkglen)), sizeof(int));
	m_iDataLen += sizeof(int);

	memcpy(m_pData + m_iDataLen, (char*)(&(packetHeader.iPacketID)), sizeof(int));
	m_iDataLen += sizeof(int);

	//m_iPointPos = sizeof(CPacketHead);
	//m_iDataLen = 0;
	m_tRenctRevPacketTime = 0;
	m_tCreateTime = 0;
}

void CPacket::SetPacketHead(CPacketHead& packHeader)
{
	m_stPacketHeader = packHeader;
	m_bSetHeader = true;

	memcpy(m_pData, (char*)(&(packHeader.iKey)), sizeof(int));
	memcpy(m_pData + m_iDataLen, (char*)(&(packHeader.pkglen)), sizeof(int));
	memcpy(m_pData + m_iDataLen, (char*)(&(packHeader.iPacketID)), sizeof(int));
}

void CPacket::SetPacketData(LPVOID pData, unsigned int iLen, bool bHasHeader)
{
	if (bHasHeader)
	{
		int iPacketLen = *((int*)((char*)pData+sizeof(int)));
		if (iLen != iPacketLen)
		{
			return;
		}
		int iKey = *((int*)((char*)pData));
		int iPacketID = *((int*)(((char*)pData) + sizeof(int)* 2));

		m_stPacketHeader.iKey = iKey;
		m_stPacketHeader.iPacketID = iPacketID;
		m_stPacketHeader.pkglen = iPacketLen;

		//m_iDataLen += sizeof(m_stPacketHeader);

		memcpy(m_pData, (char*)pData, iLen);
		m_iDataLen += iPacketLen-sizeof(CPacketHead);
	}
	else
	{
		memcpy(m_pData + m_iDataLen, (char*)pData, iLen);
		m_iDataLen += iLen;
	}
}

char* CPacket::GetData()
{
	return m_pData;
}
int CPacket::GetDataLen()
{
	return m_iDataLen;
}

int CPacket::GetBodyDataLen()
{
	return m_iDataLen - sizeof(CPacketHead);
}

void CPacket::GetValue(int& iValue)
{
	char szBufVal[sizeof(int)+1] = {0};
	memcpy(szBufVal, m_pData + m_iPointPos,sizeof(int));

	iValue = *((int*)szBufVal);
	m_iPointPos += sizeof(int);
}

void CPacket::GetValue(long& lValue)
{
	char szBufVal[sizeof(long)+1] = { 0 };
	memcpy(szBufVal, m_pData + m_iPointPos, sizeof(long));

	lValue = *((long*)szBufVal);
	m_iPointPos += sizeof(long);
}

void CPacket::GetPacketHeader(CPacketHead& packHeader)
{
	memset(&packHeader, 0, sizeof(CPacketHead));
	memcpy(&packHeader, m_pData + m_iPointPos, sizeof(CPacketHead));
	m_iPointPos += sizeof(CPacketHead);
}

void CPacket::GetPacketData(LPVOID pData, unsigned int iLen)
{
	memset(pData, 0, iLen);
	memcpy(pData, m_pData + m_iPointPos + sizeof(int), iLen);
	m_iPointPos += iLen + sizeof(int);
}

void CPacket::MakePacketHeader(char* pData, unsigned int uiDataLen)
{
	//memset(pData);
}

void CPacket::PushBack(char* szData, int iDataLen)
{
	if (!szData)
	{
		return;
	}
	if (iDataLen > (MAX_PACKET_LEN - m_iDataLen))
	{
		iDataLen = MAX_PACKET_LEN - m_iDataLen;
	}
	memcpy(m_pData + m_iDataLen, (char*)(&iDataLen), sizeof(int));
	memcpy(m_pData + m_iDataLen + sizeof(int), szData, iDataLen);
	m_iDataLen += sizeof(int) + iDataLen;
	//m_iPointPos += iDataLen;
}

void CPacket::PushBack(int iValue)
{
	if (m_iDataLen > (MAX_PACKET_LEN - sizeof(int))) return;

	memcpy(m_pData + m_iDataLen, (char*)(&iValue), sizeof(int));
	m_iDataLen += sizeof(int);
}

void CPacket::PushBack(long lValue)
{
	if (m_iDataLen > (MAX_PACKET_LEN - sizeof(long))) return;

	memcpy(m_pData + m_iDataLen, (char*)(&lValue), sizeof(long));
	//m_iPointPos += sizeof(long);
	m_iDataLen += sizeof(long);
}

void CPacket::PushBack(float fValue)
{
	if (m_iDataLen > (MAX_PACKET_LEN - sizeof(float))) return;

	memcpy(m_pData + m_iDataLen, (char*)(&fValue), sizeof(float));
	//m_iPointPos += sizeof(float);
	m_iDataLen += sizeof(float);
}

void CPacket::PushBack(double dValue)
{
	if (m_iDataLen > (MAX_PACKET_LEN - sizeof(double))) return;

	memcpy(m_pData + m_iDataLen, (char*)(&dValue), sizeof(double));
	//m_iPointPos += sizeof(double);
	m_iDataLen += sizeof(double);
}

//void CPacket::PushBack(__int32 i32Value)
//{
//	if (m_iDataLen > (MAX_PACKET_LEN - sizeof(__int32))) return;
//
//	memcpy(m_pData, (char*)(&i32Value), sizeof(__int32));
//	m_iPointPos += sizeof(__int32);
//	m_iDataLen += sizeof(__int32);
//}


CProtoPacket::CProtoPacket() :m_iDataLen(0)
{
	memset(m_pData, 0, MAX_PACKET_LEN + 8);

	CPacketHead packetHeader;
	//初始化包头位置
	memcpy(m_pData, (char*)(&(packetHeader.iKey)), sizeof(int));
	m_iDataLen += sizeof(int);
	memcpy(m_pData + m_iDataLen, (char*)(&(packetHeader.pkglen)), sizeof(int));
	m_iDataLen += sizeof(int);

	memcpy(m_pData + m_iDataLen, (char*)(&(packetHeader.iPacketID)), sizeof(int));
	m_iDataLen += sizeof(int);
	//

	//m_bSetHeader = false;
}

CProtoPacket::~CProtoPacket()
{
}

CProtoPacket::CProtoPacket(CPacketHead& packetHeader)
{
	m_iDataLen = 0;

	memset(m_pData, 0, MAX_PACKET_LEN + 8);

	memcpy(m_pData, (char*)(&(packetHeader.iKey)), sizeof(int));
	m_iDataLen += sizeof(int);
	memcpy(m_pData + m_iDataLen, (char*)(&(packetHeader.pkglen)), sizeof(int));
	m_iDataLen += sizeof(int);

	memcpy(m_pData + m_iDataLen, (char*)(&(packetHeader.iPacketID)), sizeof(int));
	m_iDataLen += sizeof(int);
}

void CProtoPacket::AddHeadPacketLen(int strLen)
{
	m_iDataLen += strLen;
	memcpy(m_pData + sizeof(int), (char*)(&(m_iDataLen)), sizeof(int));
}

CProtoPacket::CProtoPacket(int iPacketID)
{
	CPacketHead packetHeader;
	packetHeader.iKey = CHECKSTX;
	packetHeader.pkglen = sizeof(CPacketHead);
	packetHeader.iPacketID = iPacketID;

	m_iDataLen = 0;

	memset(m_pData, 0, MAX_PACKET_LEN + 8);

	memcpy(m_pData, (char*)(&(packetHeader.iKey)), sizeof(int));
	m_iDataLen += sizeof(int);
	memcpy(m_pData + m_iDataLen, (char*)(&(packetHeader.pkglen)), sizeof(int));
	m_iDataLen += sizeof(int);

	memcpy(m_pData + m_iDataLen, (char*)(&(packetHeader.iPacketID)), sizeof(int));
	m_iDataLen += sizeof(int);
}


void CProtoPacket::SetHeadPacket(CPacketHead& head)
{
	memcpy(m_pData, (char*)(&(head.iKey)), sizeof(int));
	memcpy(m_pData + sizeof(int), (char*)(&(head.pkglen)), sizeof(int));
	memcpy(m_pData + sizeof(int)* 2, (char*)(&(head.iPacketID)), sizeof(int));
}

void CProtoPacket::GetPacketObject(char* pBufferSize, int iPacketLen, int PacketID, void* pObject)
{

}

void CProtoPacket::PushBack(const char* szData, int iDataLen)
{
	if (!szData)
	{
		return;
	}
	if (iDataLen > (MAX_PACKET_LEN - m_iDataLen))
	{
		iDataLen = MAX_PACKET_LEN - m_iDataLen;
	}

	memcpy(m_pData + sizeof(CPacketHead), szData, iDataLen);

	//更改包头中的大小
	AddHeadPacketLen(iDataLen);

}





