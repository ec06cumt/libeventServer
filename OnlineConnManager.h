
#include "CLibEventData.h"
#include <hash_map>

using namespace stdext;

class COnlineConnManager
{
public:
	static COnlineConnManager* GetInstance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new COnlineConnManager();
		}
		return m_pInstance;
	}
	void AddConn(_Conn* conn);
	void RemoveConn(evutil_socket_t fd);

	_Conn* GetConn(evutil_socket_t fd);

	void SendPacketToClent(evutil_socket_t fd,const char* buf,int iBufSize);
	virtual ~COnlineConnManager(void);
private:
	COnlineConnManager(void);
	
	hash_map<evutil_socket_t, _Conn*> m_hashMapConn;

	static COnlineConnManager* m_pInstance;

};

#define GetOnlineConnMgr() (COnlineConnManager::GetInstance())