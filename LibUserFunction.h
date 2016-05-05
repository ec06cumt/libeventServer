#pragma once
#include "CLibEventData.h"

class CLibUserFunction
{
public:
	CLibUserFunction(void);
	~CLibUserFunction(void);
public:
	static int Connect(Conn* c);
	static int DisConnect(Conn* c);
	static int TimeOut(Conn* c);
	static int Login(Conn* c);
	static int Logout(Conn* c);
};
//�ص�����ָ��
typedef int (*lpFunPtr)(Conn* c);
