
#ifndef _CPACKET_H_
#define _CPACKET_H_

#include <string>
#include <math.h>
#include <WinSock2.h>
//#include "..\CProtocol.h"
#include "..\..\CProtocol.h"

#define MAX_PACKET_LEN 8192

/************************************************************************/
/* 包头                                                                 */
/************************************************************************/
typedef struct _CPacketHead
{
	int iKey;			//check key设置
	int pkglen;			//包的长度
	int iPacketID;		//包的协议ID
	_CPacketHead()
	{
		iKey = 0;
		pkglen = 0;
		iPacketID = 0;
	}
}CPacketHead, *PCPacketHead;

class CPacket
{
public:
	CPacket();
	CPacket(CPacketHead& packetHeader);
	virtual ~CPacket();
	unsigned int GetReadPoint() { return m_iPointPos; };
	bool GetSetHeader() { return m_bSetHeader; };
	void MakePacketHeader(char* pData,unsigned int uiDataLen);

	void SetPacketHead(CPacketHead& packHeader);
	void SetPacketData(LPVOID pData, unsigned int iLen,bool bHasHeader = false);

	void PushBack(char* szData,int iDataLen);
	void PushBack(int iValue);
	void PushBack(long lValue);
	void PushBack(short sValue);
	void PushBack(float fValue);
	void PushBack(double dValue);
	//void PushBack(UINT u8Value);
	//void PushBack(__int32 i32Value);

	void GetValue(int& iValue);
	void GetValue(long& lValue);

	void GetPacketHeader(CPacketHead& packHeader);

	void GetPacketData(LPVOID pData,unsigned int iLen);

	char* GetData();
	int GetDataLen();
	int GetBodyDataLen();
protected:
	CPacketHead m_stPacketHeader;	//包头
	time_t m_tRenctRevPacketTime;	//最近收到包的时间
	time_t m_tCreateTime;			//包创建的时间
	char m_pData[MAX_PACKET_LEN+8];	//包内容
	unsigned int m_iDataLen;
	unsigned int m_iPointPos;
	bool m_bSetHeader;
};

class CProtoPacket
{
public:
	CProtoPacket(CPacketHead& packetHeader);
	CProtoPacket(int iPacketID);
	~CProtoPacket();

	char* GetData()
	{
		return m_pData;
	}

	unsigned int GetLen()
	{
		return m_iDataLen;
	}
	void AddHeadPacketLen(int strLen);
	void SetHeadPacket(CPacketHead& head);
	void GetPacketObject(char* pBufferSize, int iPacketLen, int PacketID, void* pObject);
	void PushBack(const char* szData, int iDataLen);
private:
	CProtoPacket();
	char m_pData[MAX_PACKET_LEN + 8];	//包内容
	unsigned int m_iDataLen;
	//bool m_bSetHeader;
};

#endif