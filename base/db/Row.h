
#pragma once

//typedef	unsigned int SOCKET;
//
//#ifndef	my_socket_defined
//#define	my_socket SOCKET
//#endif
#include <Windows.h>
#include <Winsock.h>
#include <mysql.h>
#define T_RESULT MYSQL_RES
#define T_ROW    MYSQL_ROW



class CRow
{
public:
	CRow(T_RESULT*, T_ROW);
	~CRow();

	char* GetField(int index);
	int GetFieldCount();
	int IsClosed() const;
	void Close();

private:
	T_RESULT* m_pResult;
	T_ROW m_Fields;
};