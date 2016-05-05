
#include "..\stdafx.h"
#include "PacketUtil.h"


char* CPacketManager::MakePacket(CPacket& packet)
{
	char szPacketBuf[MAX_PACKET_LEN + 8] = {0};
	bool bHasHeader = packet.GetSetHeader();
	if (!bHasHeader)
	{
		return NULL;
	}

	return NULL;

};

int CPacketManager::SendPacket(SOCKET sock, CPacket& packet)
{
    int rs = send(sock, packet.GetData(), packet.GetDataLen(), 0);
	return rs == packet.GetDataLen();
}

int CPacketManager::RevPacket(SOCKET sock, CPacket& packet)
{
	char szBuf[MAX_PACKET_LEN] = {0};

	int rs = recv(sock, szBuf, MAX_PACKET_LEN, 0);
	if (rs > 0 && szBuf)
	{
		int iPacketLen = *((int*)(szBuf + sizeof(int)));
		if (iPacketLen > rs)
		{
			int iRs2 = recv(sock, szBuf + rs, MAX_PACKET_LEN - rs, 0);
			if (iRs2 < 0)
			{
				return -1;
			}
		}
		int iKey = *((int*)szBuf);
		int iPacketID = *((int*)(szBuf+sizeof(int)*2));
		CPacketHead packetHead;
		packetHead.iKey = iKey;
		packetHead.pkglen = iPacketLen;
		packetHead.iPacketID = iPacketID;

		packet.SetPacketData(szBuf, iPacketLen,true);

		return 0;

	}
	else
		return -1;

}

void CPacketManager::SetRecivePacketFlag(bool bLoop)
{
	m_bLoopRev = bLoop;
}

void CPacketManager::Process(SOCKET sock)
{
	while (m_bLoopRev)
	{
		CPacket packet;
		int iRs = RevPacket(sock, packet);
		if (iRs == 0)
		{

		}
	}
}