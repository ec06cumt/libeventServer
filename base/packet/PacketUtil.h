
#ifndef _CPACKETUTIL_H_
#define _CPACKETUTIL_H_

#include <string.h>
#include "Singleton.h"
//#include "boostSingleton.h"
#include "Packet.h"
#include <queue>



class CPacketManager:public Singleton<CPacketManager>
{
public:

	char* MakePacket(CPacket& packet);
	int SendPacket(SOCKET sock, CPacket& packet);

	int RevPacket(SOCKET sock, CPacket& packet);
	void SetRecivePacketFlag(bool bLoop);

	void Process(SOCKET sock);

private:
	CPacketManager(){};
	~CPacketManager(){};

	friend  Singleton<CPacketManager>;
	friend class auto_ptr<CPacketManager>;

	CPacketManager(const CPacketManager&);
	CPacketManager& operator =(const CPacketManager&);

	bool m_bLoopRev;
};

//class CPacketManager2
//{
//public:
//	void do_something(){};
//private:
//	CPacketManager2(){};
//	~CPacketManager2(){};
//	friend class BoostSingleton<CPacketManager2>;
//};

#endif
