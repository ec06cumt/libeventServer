
#pragma once
#include <map>
#include "..\lock\LockObject.h"
#include "..\base\lock\GuardLock.h"

template <class T_LPPACKET_PROC>
class CPacketProcManager
{
public:
	CPacketProcManager(){};
	virtual ~CPacketProcManager();

	T_LPPACKET_PROC FindProc(const UINT packetID);
	void Register(const UINT packetID, const T_LPPACKET_PROC proc);

private:
	//typedef std::map<UINT, T_LPPACKET_PROC> PACKET_PROC_MAP;
	std::map<UINT, T_LPPACKET_PROC> m_mapProc;
	CLockObject m_lockObj;
};

template<class T_LPPACKET_PROC>
inline CPacketProcManager<T_LPPACKET_PROC>::~CPacketProcManager()
{
	if (!m_mapProc.empty())
	{
		m_mapProc.clear();
	}
}

template<class T_LPPACKET_PROC>
inline  void CPacketProcManager<T_LPPACKET_PROC>::Register(const UINT packetID, const T_LPPACKET_PROC proc)
{
	m_mapProc.insert(std::map<UINT, T_LPPACKET_PROC>::value_type(packetID, (T_LPPACKET_PROC)proc));
}

template<class T_LPPACKET_PROC>
inline T_LPPACKET_PROC CPacketProcManager<T_LPPACKET_PROC>::FindProc(const UINT packetID)
{
	CGuardLock guard(&m_lockObj);
	std::map<UINT, T_LPPACKET_PROC>::iterator iter = m_mapProc.find(packetID);

	if (iter != m_mapProc.end())
	{
		return iter->second;
	}
	return NULL;
}
