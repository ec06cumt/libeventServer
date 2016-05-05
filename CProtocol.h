#pragma once
typedef enum HeadStx
{
	CHECKSTX = 860805
};
typedef enum emConfig
{
	emMaxBuffLen = 8192,
	emMaxPackageType = 65535
};
//�����
typedef enum emFunID
{
	emFunNull = -1,
	emFunClosed = 0,
	emFunTimeout = 1,
	emFunError = 2,
	emFunConnected = 3,
	emFunDisConnected = 4,

	emFunBase = 1000,
	//emFunLogin = emFunBase + 1,
	LOGIN_CS_IN = emFunBase + 1,
	//int uid;	//�û���

	LOGIN_SC_IN = emFunBase + 2,
	//int uid;

	LOGIN_CS_OUT = emFunBase + 3,

	LOGIN_SC_OUT = emFunBase + 4,

	CONNECT_SC_CLOSE = emFunBase + 5,

	emFunLogout = emFunBase + 6
};

typedef enum emFunReturn
{
	emFunReturClose=1,
	emFunReturnSend,
	emFunReturnRecv
};
/************************************************************************/
/* ��ͷ                                                                 */
/************************************************************************/
typedef struct _Head 
{
	int stx;
	int pkglen;
	int nFunID;
	_Head()
	{
		stx = 0;
		pkglen = 0;
		nFunID = 0;
	}
}Head,*PHead;

/************************************************************************/
/* �û���¼Э��                                                         */
/************************************************************************/
typedef struct _Proto_Login_Req
{
	int uid;	//�û���
}Proto_Login_Req,*PProto_Login_Req;

typedef struct _Proto_Login_Rsp
{
	int uid;
}Proto_Login_Rsp,*PProto_Login_Rsp;


/************************************************************************/
/* �ǳ�Э��                                                             */
/************************************************************************/
typedef struct _ProtoLogout_Req
{
}ProtoLogout_Req,*PProtoLogout_Req;