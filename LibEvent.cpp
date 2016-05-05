#include "StdAfx.h"
#include "LibEvent.h"

#include <string>
#include <iostream>
using namespace std;

#include <assert.h>
#include <signal.h>

#include <WinSock2.h>
#pragma comment (lib,"ws2_32.lib")
#pragma comment (lib,"wsock32.lib")

#include "LibEventFunction.h"
#include "OnlineConnManager.h"

#ifdef _DEBUG
#pragma comment (lib,".\\libevent\\libevent.lib")
#else
#pragma comment (lib,".\\libevent\\libevent.lib")
#endif

CLibEvent::CLibEvent(void)
{	
	LoadFuns();
	ZeroMemory(&m_Server,sizeof(m_Server));
	WSADATA WSAData;
	WSAStartup(0x0201, &WSAData);
}

CLibEvent::~CLibEvent(void)
{
	WSACleanup();
}

bool CLibEvent::StartServer(int port, short workernum, unsigned int connnum, int read_timeout, int write_timeout)
{	
	m_Server.bStart=false;
	m_Server.nCurrentWorker=0;
	m_Server.nPort=port;
	m_Server.workernum=workernum;
	m_Server.connnum=connnum;
	m_Server.read_timeout=read_timeout;
	m_Server.write_timeout=write_timeout;
	evthread_use_windows_threads();
	m_Server.pBase=event_base_new();
	if (m_Server.pBase==NULL)
	{
		return false;
	}
	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(m_Server.nPort);
	m_Server.pListener = evconnlistener_new_bind(m_Server.pBase,DoAccept,(void*)&m_Server,LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE,-1,(struct sockaddr*)&sin,sizeof(sin));
	if (m_Server.pListener==NULL)
	{
		return false;
	}	

	m_Server.pWorker=new Worker[workernum];

	for (int i=0;i< workernum;i++)
	{
		m_Server.pWorker[i].pWokerbase = event_base_new();
		if (m_Server.pWorker[i].pWokerbase == NULL)
		{
			delete []m_Server.pWorker;
			return false;
		}
		//初始化连接对象
		{
			m_Server.pWorker[i].pListConn = new ConnList();
			if (m_Server.pWorker[i].pListConn==NULL)
			{
				return false;
			}
			m_Server.pWorker[i].pListConn->plistConn=new Conn[m_Server.connnum+1];
			m_Server.pWorker[i].pListConn->head=&m_Server.pWorker[i].pListConn->plistConn[0];
			m_Server.pWorker[i].pListConn->tail=&m_Server.pWorker[i].pListConn->plistConn[m_Server.connnum];
			for (int j=0; j<m_Server.connnum; j++) {
				m_Server.pWorker[i].pListConn->plistConn[j].index=j;
				m_Server.pWorker[i].pListConn->plistConn[j].next=&m_Server.pWorker[i].pListConn->plistConn[j+1];
			}
			m_Server.pWorker[i].pListConn->plistConn[m_Server.connnum].index=m_Server.connnum;
			m_Server.pWorker[i].pListConn->plistConn[m_Server.connnum].next=NULL;
			//设置当前事件
			Conn *p=m_Server.pWorker[i].pListConn->head;
			while (p!=NULL)
			{
				p->bufev = bufferevent_socket_new(m_Server.pWorker[i].pWokerbase,-1, BEV_OPT_CLOSE_ON_FREE);
				if (p->bufev == NULL) 
				{
					return false;
				}
				bufferevent_setcb(p->bufev, DoRead, NULL, DoError, p);
				bufferevent_setwatermark(p->bufev, EV_READ, 0, emMaxBuffLen);
				bufferevent_enable(p->bufev, EV_READ|EV_WRITE);

				//struct timeval delayWriteTimeout;
				//delayWriteTimeout.tv_sec = m_Server.write_timeout;
				//delayWriteTimeout.tv_usec = 0;

				//struct timeval delayReadTimeout;
				//delayReadTimeout.tv_sec = m_Server.read_timeout;
				//delayReadTimeout.tv_usec = 0;

				//bufferevent_set_timeouts(p->bufev,&delayReadTimeout,&delayWriteTimeout);

				p->owner = &m_Server.pWorker[i];
				p = p->next;
			}
		}
		m_Server.pWorker[i].hThread=CreateThread(NULL,0,ThreadWorkers,&m_Server.pWorker[i],0,NULL);
		//printf("",);
	}
	m_Server.hThread=CreateThread(NULL,0,ThreadServer,&m_Server,0,NULL);
	if (m_Server.hThread==NULL)
	{
		return false;
	}
	m_Server.bStart=true;
	return true;
}

void CLibEvent::SetConnTimeOut(struct bufferevent *bev, timeval& timeOutRead, timeval& timeOutWrite)
{

}

void CLibEvent::StopServer()
{
	if (m_Server.bStart)
	{
		struct timeval delay = { 2, 0 };
		event_base_loopexit(m_Server.pBase, &delay);
		WaitForSingleObject(m_Server.hThread,INFINITE);
		if (m_Server.pWorker)
		{
			for (int i=0;i<m_Server.workernum;i++)
			{
				event_base_loopexit(m_Server.pWorker[i].pWokerbase, &delay);
				WaitForSingleObject(m_Server.pWorker[i].hThread,INFINITE);
			}
			for (int i=0;i<m_Server.workernum;i++)
			{
				if (m_Server.pWorker[i].pListConn)
				{
					delete []m_Server.pWorker[i].pListConn->plistConn;
					delete m_Server.pWorker[i].pListConn;
					m_Server.pWorker[i].pListConn=NULL;
				}
				event_base_free(m_Server.pWorker[i].pWokerbase);
			}			
			delete[]m_Server.pWorker;
			m_Server.pWorker=NULL;
		}	
		evconnlistener_free(m_Server.pListener);
		event_base_free(m_Server.pBase);
	}
	m_Server.bStart=false;	

//删除所有的单例模式;
	DeleteSingleton();
}

void CLibEvent::DoRead(struct bufferevent *bev, void *ctx)
{
    struct evbuffer * input=bufferevent_get_input(bev);
    if (evbuffer_get_length(input)) 
	{
		Conn *c = (Conn*) ctx;
        while (evbuffer_get_length(input)) 
		{
			c->in_buf_len += evbuffer_remove(input, c->in_buf,emMaxBuffLen-c->in_buf_len);         
        }		
		while(true)
		{
			//一个协议包的请求头还没读完，则继续循环读或者等待下一个libevent时间进行循环读
			if (c->in_buf_len < sizeof(CPacketHead))
			{
				return;
			}
			char szHead[sizeof(CPacketHead)] = { 0 };
			memcpy(szHead, c->in_buf, sizeof(CPacketHead));
			CPacketHead* h = (CPacketHead*)szHead;

			if (h->pkglen >emMaxBuffLen - sizeof(CPacketHead) || h->pkglen<sizeof(CPacketHead) || h->iKey != CLibEventFunction::LoadStx())
			{
				//请求包不合法
				CloseConn(c);
				return;
			}
			//读取的数据不够
			if (c->in_buf_len < h->pkglen) 
			{
				return;
			}
			
			int iRet = DispatchFunction(bev, h, c);
			if (iRet < 0)
			{
				return;
			}
			//处理下一个协议包，或者继续读
			c->in_buf_len -= h->pkglen;
			if (c->in_buf_len == 0)
			{
				break;
			}
			else
			{
				assert(c->in_buf_len>0);
				memmove(c->in_buf,c->in_buf + h->pkglen,c->in_buf_len);
			}
		}
    }
}

int CLibEvent::DispatchFunction(struct bufferevent *bev, CPacketHead* header, Conn *pConn)
{
	if (!header || !bev || !pConn)
	{
		return -1;
	}
	//执行协议指令
	if (header->iPacketID > emFunBase && CLibEventFunction::DispatchFunction(header->iPacketID, NULL))
	{
		switch (CLibEventFunction::DispatchFunction(header->iPacketID, pConn))
		{
		case LOGIN_SC_IN:
		{
			struct evbuffer * output = bufferevent_get_output(bev);
			evbuffer_add(output, pConn->out_buf, pConn->out_buf_len);
			printf("LOGIN_SC_IN \n");
			break;
		}
		case CONNECT_SC_CLOSE:
		{
			CloseConn(pConn);
			return -1;
		}
		case emFunReturClose:
		{
			CloseConn(pConn);
			return -1;
		}
		case emFunReturnRecv:
		{
			printf("Recv data");
			break;
		}
		}
	}
	else
	{
		CloseConn(pConn);
		return -1;
	}
	return 0;
}

void CLibEvent::CloseConn(Conn *pConn,int nFunID)
{
	printf("CLibEvent::CloseConn address= %d,fd=%d nFunID = %d\n", pConn,pConn->fd, nFunID);

	pConn->in_buf_len = 0;
	CLibEventFunction::DispatchFunction(nFunID,pConn);

	bufferevent_disable(pConn->bufev, EV_READ | EV_WRITE);
	evutil_closesocket(pConn->fd);
	//在线删除链接
	GetOnlineConnMgr()->RemoveConn(pConn->fd);
	pConn->owner->PutFreeConn(pConn);
	
}

void CLibEvent::CloseConn(Conn *pConn)
{
	printf("CLibEvent::CloseConn fd=%d\n",pConn->fd);
	//CloseConn(pConn,emFunClosed);
}

void CLibEvent::TimeOut(Conn *pConn, int nFunID)
{
	printf("CLibEvent::TimeOut fd=%d,nFunID = %d\n", pConn->fd, nFunID);
}
void CLibEvent::TimeOut(Conn *pConn)
{
	printf("CLibEvent::TimeOut fd=%d\n", pConn->fd);
	TimeOut(pConn, emFunTimeout);
}

void CLibEvent::DoError(struct bufferevent *bev, short error, void *ctx)
{
	Conn *c=(Conn*)ctx;
	emFunID id=emFunNull;
	if (error&EVBUFFER_TIMEOUT) 
	{
		id = emFunTimeout;
	} 
	else if (error&EVBUFFER_EOF)
	{
		id = emFunClosed;
	} 
	else if (error&EVBUFFER_ERROR) 
	{
		id= emFunError;
	}

	printf("CLibEvent::DoError errorID = %d\n", id);

	//CloseConn(c, id);
}

void CLibEvent::DoAccept(struct evconnlistener *listener, evutil_socket_t fd,struct sockaddr *sa, int socklen, void *user_data)
{
	static int iNumAccept = 0;
	iNumAccept++;
	printf("ClibEvent Accept=%d\n", iNumAccept);
	//此处为监听线程的event.不做处理.
	Server *pServer = (Server *)user_data;
	//主线程处做任务分发.
	int nCurrent=pServer->nCurrentWorker++%pServer->workernum;
	//当前线程所在ID号
	Worker &pWorker=pServer->pWorker[nCurrent];
	printf("ClibEvent pServer->pWorker[nCurrent]=%d\n", nCurrent);
	//通知线程开始读取数据,用于分配哪一个线程来处理此处的event事件
	Conn *pConn = GetOnlineConnMgr()->GetConn(fd);
	if (!pConn)
	{
		pConn = pWorker.GetFreeConn();
		printf("Connection address:%d\n", pConn);
		if (pConn == NULL)
		{
			return;
		}

		pConn->fd=fd;
		evutil_make_socket_nonblocking(pConn->fd);
		bufferevent_setfd(pConn->bufev, pConn->fd);
		//添加到在线链接
		GetOnlineConnMgr()->AddConn(pConn);
	}
	//转发发送事件
	CLibEventFunction::DispatchFunction(emFunConnected,pConn);
	bufferevent_enable(pConn->bufev, EV_READ | EV_WRITE);
}

DWORD WINAPI CLibEvent::ThreadServer(LPVOID lPVOID)
{
	Server * pServer=reinterpret_cast<Server *>(lPVOID);
	if (pServer==NULL)
	{
		return -1;
	}
	event_base_dispatch(pServer->pBase);
	return GetCurrentThreadId();
}

DWORD WINAPI CLibEvent::ThreadWorkers(LPVOID lPVOID)
{
	Worker *pWorker=reinterpret_cast<Worker *>(lPVOID);
	if (pWorker==NULL)
	{
		return -1;
	}
	printf("server LibEvent ThreadID =%d\n", GetCurrentThreadId());
	event_base_dispatch(pWorker->pWokerbase);
	
	return GetCurrentThreadId();
}

void CLibEvent::LoadFuns()
{
	CLibEventFunction::RegistFunc(LOGIN_CS_IN, CLibUserFunction::Login);
	CLibEventFunction::RegistFunc(LOGIN_CS_OUT, CLibUserFunction::Logout);

	CLibEventFunction::RegistFunc(emFunConnected, CLibUserFunction::Connect);
	CLibEventFunction::RegistFunc(emFunTimeout, CLibUserFunction::TimeOut);

	CLibEventFunction::RegistDisconnectedFunc(CLibUserFunction::DisConnect);
	CLibEventFunction::RegistStx(CHECKSTX);

	 
}

void CLibEvent::DeleteSingleton()
{
	delete GetOnlineConnMgr();
}