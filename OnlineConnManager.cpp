
#include "stdafx.h"
#include "OnlineConnManager.h"

COnlineConnManager* COnlineConnManager::m_pInstance = new COnlineConnManager;

COnlineConnManager::COnlineConnManager()
{
}

COnlineConnManager::~COnlineConnManager()
{
	m_hashMapConn.clear();
}

void COnlineConnManager::AddConn(_Conn* conn)
{
	if (!conn)
		return;
	hash_map<evutil_socket_t, _Conn*>::iterator it = m_hashMapConn.find(conn->fd);
	if (it == m_hashMapConn.end())
	{
		m_hashMapConn.insert(std::make_pair(conn->fd, conn));
	}
	else
		m_hashMapConn[conn->fd] = conn;
}

void COnlineConnManager::RemoveConn(evutil_socket_t fd)
{
	hash_map<evutil_socket_t, _Conn*>::iterator it = m_hashMapConn.find(fd);
	if (it == m_hashMapConn.end())
	{
		return;
	}
	else
		m_hashMapConn.erase(fd);
}

_Conn* COnlineConnManager::GetConn(evutil_socket_t fd)
{
	hash_map<evutil_socket_t, _Conn*>::iterator it = m_hashMapConn.find(fd);
	if (it == m_hashMapConn.end())
	{
		return NULL;
	}
	else
		return m_hashMapConn[fd];
}

void COnlineConnManager::SendPacketToClent(evutil_socket_t fd, const char* buf, int iBufSize)
{
	if (!buf || iBufSize == 0 || iBufSize > emMaxBuffLen)
	{
		return;
	}
	Conn *pConn = GetConn(fd);
	if (pConn)
	{
		if (pConn->bufev)
		{
			struct evbuffer * output = bufferevent_get_output(pConn->bufev);
			evbuffer_add(output, buf, iBufSize);
			printf("LOGIN_SC_IN has send!\n");
		}
	}
}