
#ifndef _CDBMYSQL_H_
#define _CDBMYSQL_H_
#include "winsock.h"
#include "Result.h"

#pragma comment(lib,"libmySQL.lib")

class CDBMySql
{
public:
	CDBMySql(const char* host, const char* dataBase);
	CDBMySql(const char*, const char*, const char*, const char*);
	~CDBMySql();

	int Connect(const char* szhost,const char* szDatabase,const char* szUserID,const char* szPWD);
	void Close();
	int GetAffectedRows() const;
	const char* GetError();
	int IsConnected() const;
	CResult* Query(const char* sqlCommand);

private:
	int m_iAffectedRows;
	MYSQL m_Mysql;
	MYSQL* m_pConnection;
};

#endif