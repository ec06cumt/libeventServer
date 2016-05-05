#pragma once
#include <event2/bufferevent.h>
#include <event2/bufferevent_compat.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>
#include <event2/http.h>
#include <event2/buffer_compat.h>
#include <event2/http_struct.h>
#include <event2/bufferevent.h>
#include <event2/thread.h>

#include "CProtocol.h"

#include "base\packet\PacketUtil.h"
#include "base\packet\Packet.h"
//#include <hash_map>

//using namespace stdext;
struct _Conn;
struct _Worker;

//服务器属性封装对象
struct _Server
{
	bool bStart;					//服务器是否已经启动
	short nPort;					//服务器端口
	short workernum;				//工作线程
	unsigned int connnum;			//每个工作线程，可以连接的数量
	volatile int nCurrentWorker;	//当前线程的Worker数量
	int read_timeout;				//读数据的超时时间
	int write_timeout;				//写数据的超时时间
	struct evconnlistener *pListener;	//监听事件
	struct event_base *pBase;			//服务器主程事件组
	HANDLE hThread;						//服务器线程
	_Worker *pWorker;					//工作线程表		
};
//连接对象列表
struct _ConnList
{
	_ConnList()
	{
		head=NULL;
		tail=NULL;
		plistConn=NULL;
	}
	_Conn *head;
	_Conn *tail;
	_Conn *plistConn;
};
//连接对象
struct _Conn
{
	_Conn()
	{
		fd=NULL;
		bufev=NULL;
		index=-1;
		in_buf_len=0;
		out_buf_len=0;
		owner = NULL;
		next = NULL;
		in_buf = new char[emMaxBuffLen];
		out_buf = new char[emMaxBuffLen];
	}
	~_Conn()
	{
		delete[]in_buf;
		delete[]out_buf;
		bufferevent_free(bufev);
	}
	struct bufferevent *bufev;
	evutil_socket_t fd;
	int index;
	char *in_buf;
	short in_buf_len;
	char *out_buf;
	short out_buf_len;
	_Worker *owner;
	_Conn *next;
};
//工作线程封装对象.
struct _Worker
{
	_Worker()
	{
		pWokerbase=NULL;
		hThread = INVALID_HANDLE_VALUE;
		pListConn=NULL;
	}
	struct event_base *pWokerbase;
	HANDLE hThread;
	_ConnList *pListConn;
	inline _Conn* GetFreeConn()
	{
		_Conn* pItem=NULL;
		if(pListConn->head != pListConn->tail)
		{
			pItem=pListConn->head;

			printf("pItem addr:%d\n", pItem);

			pListConn->head = pListConn->head->next;
		}
		if (pItem == NULL)
		{
			printf("connection reach max connection num\n");
		}
		return pItem;
	}
	inline void PutFreeConn(_Conn *pItem)
	{
		if (pItem)
		{
			printf("Close Conn Free Connecton! fd=%d\n", pItem->fd);
		}
		pListConn->tail->next=pItem;
		pListConn->tail=pItem;
	}
};

typedef struct _Server Server;
typedef struct _Worker Worker;
typedef struct _Conn Conn;
typedef struct _ConnList ConnList;
