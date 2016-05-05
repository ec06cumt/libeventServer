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

//���������Է�װ����
struct _Server
{
	bool bStart;					//�������Ƿ��Ѿ�����
	short nPort;					//�������˿�
	short workernum;				//�����߳�
	unsigned int connnum;			//ÿ�������̣߳��������ӵ�����
	volatile int nCurrentWorker;	//��ǰ�̵߳�Worker����
	int read_timeout;				//�����ݵĳ�ʱʱ��
	int write_timeout;				//д���ݵĳ�ʱʱ��
	struct evconnlistener *pListener;	//�����¼�
	struct event_base *pBase;			//�����������¼���
	HANDLE hThread;						//�������߳�
	_Worker *pWorker;					//�����̱߳�		
};
//���Ӷ����б�
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
//���Ӷ���
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
//�����̷߳�װ����.
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
