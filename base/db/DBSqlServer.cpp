
#include "..\stdafx.h"
#include <string.h>

#include <assert.h>
#include "DBSqlServer.h"
#include "..\common\Util.h"
#pragma comment(lib,"odbc32.lib")

#define SQL_MAX_CONN_COUNT 128

CDBSqlServer::CDBSqlServer() :m_henv(0), m_hdbc(0), m_hstmt(0)
							, m_nBindParm(0), m_nBindCol(0), m_retCode(0)
							, m_bConnected(false), m_nCursorErrorCount(0)
{
	memset(&m_cbValue, 0, sizeof(m_cbValue));
	memset(&m_szConnectionStr, 0, sizeof(m_szConnectionStr));

	StarUpSqlServer();
	
}

CDBSqlServer::~CDBSqlServer()
{

}


void CDBSqlServer::GetError()
{
	SQLWCHAR szbuf[1024] = {0};
	SQLWCHAR szState[100] = { 0 };
	RETCODE rc = SQL_SUCCESS;

	int nCount = 0;
	for (rc = ::SQLError(m_henv, m_hdbc, m_hstmt, szState, NULL, szbuf,1024,NULL);;
		rc = ::SQLError(m_henv, NULL, NULL, szState, NULL, szbuf, 1024, NULL))
	{
		if (rc == SQL_SUCCESS || rc == SQL_NO_DATA) break;
		if (++nCount > 10)
		{
			assert( false && "CDBSqlServer::GetError :Loop");
			return;
		}
	}
}

bool CDBSqlServer::StarUpSqlServer()
{
	//准备ODBC环境和句柄
	m_retCode = ::SQLAllocHandle(SQL_HANDLE_ENV,NULL,&m_henv);
	if (!ODBC_SUCCEED(m_retCode))
	{
		GetError();
		return false;
	}
	//提供ODBC版本
	m_retCode = ::SQLSetEnvAttr(m_henv,SQL_ATTR_ODBC_VERSION,(SQLPOINTER)SQL_OV_ODBC3,SQL_IS_INTEGER);
	if (!ODBC_SUCCEED(m_retCode))
	{
		GetError();
		return false;
	}
	//获取ODBC的连接句柄
	m_retCode = ::SQLAllocHandle(SQL_HANDLE_DBC, m_henv, &m_hdbc);
	if (!ODBC_SUCCEED(m_retCode))
	{
		GetError();
		return false;
	}

	TCHAR szResult[100];
	SWORD nResult;
	::SQLGetInfo(m_hdbc,SQL_ODBC_VER,szResult,sizeof(szResult),&nResult);

	m_nCursorErrorCount = 0;
	return true;

}

bool CDBSqlServer::ConnectSqlServer(SQLWCHAR* pszDSN, SQLWCHAR* pszPID, SQLWCHAR* pszPW)
{
	m_retCode = ::SQLConnect(m_hdbc, pszDSN, SQL_NTS, pszPID, SQL_NTS, pszPW, SQL_NTS);
	if (!ODBC_SUCCEED(m_retCode))
	{
		GetError();
		return false;
	}

	m_retCode = ::SQLAllocHandle(SQL_HANDLE_STMT, m_hdbc, &m_hstmt);
	if (!ODBC_SUCCEED(m_retCode))
	{
		GetError();
		return false;
	}

	m_bConnected = true;
	return true;
}

bool CDBSqlServer::DriverConnectSqlServer(SQLWCHAR* pszDSN)
{
	
	SQLTCHAR szOutConn[SQL_MAX_CONN_COUNT] = { 0 };
	short sOutConn = 0;

	m_retCode = ::SQLDriverConnect(m_hdbc, NULL, pszDSN, SQL_NTS, szOutConn, SQL_MAX_CONN_COUNT, &sOutConn, SQL_DRIVER_COMPLETE);
	if (!ODBC_SUCCEED(m_retCode))
	{
		GetError();
		return false;
	}

	m_retCode = ::SQLAllocHandle(SQL_HANDLE_STMT, m_hdbc, &m_hstmt);
	if (!ODBC_SUCCEED(m_retCode))
	{
		GetError();
		return false;
	}

	m_bConnected = true;
	return true;

}

void CDBSqlServer::DisconnectSqlServer()
{
	m_bConnected = false;
	if (m_hstmt)
	{
		::SQLFreeHandle(SQL_HANDLE_STMT, m_hstmt);
	}
	if (m_hdbc)
	{
		::SQLDisconnect(m_hdbc);
	}
	m_hstmt = 0;
	m_hdbc = 0;
}

void CDBSqlServer::CleanUp()
{
	if (m_hdbc)
	{
		::SQLFreeHandle(SQL_HANDLE_DBC, m_hdbc);
	}
	if (m_henv)
	{
		::SQLFreeEnv(m_henv);
	}
	m_hdbc = 0;
	m_henv = 0;
}

bool CDBSqlServer::ExecuteDirect(SQLWCHAR* sql)
{
	SQLRETURN ret ;

	ClearMoreResult(true);
	DWORD dwBegin = ::GetTickCount();
	ret = ::SQLExecDirect(m_hstmt, sql, SQL_NTS);
	if ((::GetTickCount() - dwBegin) > 1000 * 3)
	{
		//log记录执行时间长的sql
	}

	//
	if (ODBC_FAILED(ret))
	{
		//进行重连

	}
	return true;
}

bool CDBSqlServer::Fetch()
{
	m_retCode = ::SQLFetch(m_hstmt);

	if (ODBC_SUCCEED(m_retCode)) return true;

	if (m_retCode == SQL_NO_DATA)
	{
		m_retCode = ::SQLMoreResults(m_hstmt);
		if (ODBC_SUCCEED(m_retCode)) return true;
	}
	else
	{
		GetError();
		ClearMoreResult(true);
	}

	return false;

}

void CDBSqlServer::InitBindParam()
{
	m_nBindParm = 0;
	memset(&m_cbValue, 0, sizeof(m_cbValue));
#if (ODBCVER < 0x0300)
	m_retCode = ::SQLFreeStmt(m_hstmt, SQL_RESET_PARAMS);
#endif
}

SQLRETURN CDBSqlServer::BindParam(INT8& nValue, SQLSMALLINT inoutType)
{
	
	m_cbValue[++m_nBindParm] = 0;
	assert(m_nBindParm < MAX_PARM_COUNT);
	return ::SQLBindParameter(m_hstmt, m_nBindParm, inoutType, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &nValue, 0, &m_cbValue[m_nBindParm]);
}

SQLRETURN CDBSqlServer::BindParam(UINT8& nValue, SQLSMALLINT inoutType)
{

	m_cbValue[++m_nBindParm] = 0;
	assert(m_nBindParm < MAX_PARM_COUNT);
	return ::SQLBindParameter(m_hstmt, m_nBindParm, inoutType, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &nValue, 0, &m_cbValue[m_nBindParm]);
}

SQLRETURN CDBSqlServer::BindParam(int& nValue, SQLSMALLINT inoutType)
{

	m_cbValue[++m_nBindParm] = 0;
	assert(m_nBindParm < MAX_PARM_COUNT);
	return ::SQLBindParameter(m_hstmt, m_nBindParm, inoutType, SQL_C_SLONG, SQL_INTEGER, 0, 0, &nValue, 0, &m_cbValue[m_nBindParm]);
}

SQLRETURN CDBSqlServer::BindParam(char* pstr, int iParamLen,SQLSMALLINT inoutType)
{

	m_cbValue[++m_nBindParm] = SQL_NTS;

	assert(m_nBindParm < MAX_PARM_COUNT);
	return ::SQLBindParameter(m_hstmt, m_nBindParm, inoutType, SQL_C_CHAR, SQL_CHAR, iParamLen, 0, pstr, 0, &m_cbValue[m_nBindParm]);
}

void CDBSqlServer::InitBindCol()
{
	m_nBindCol = 0;
	memset(&m_cbValue, 0, sizeof(m_cbValue));
#if (ODBCVER < 0x0300)
	m_retCode = ::SQLFreeStmt(m_hstmt,SQL_UNBIND);
#endif
}

SQLRETURN CDBSqlServer::BindCol(INT8& nValue)
{
	m_nBindCol++;
	return ::SQLBindCol(m_hstmt, m_nBindCol, SQL_C_TINYINT, &nValue, sizeof(INT8), &m_cbValue[m_nBindParm]);
}

SQLRETURN CDBSqlServer::BindCol(int& nValue)
{
	m_nBindCol++;
	return ::SQLBindCol(m_hstmt, m_nBindCol, SQL_C_SLONG, &nValue, sizeof(int), &m_cbValue[m_nBindParm]);
}

SQLRETURN CDBSqlServer::BindCol(unsigned int& nValue)
{
	m_nBindCol++;
	return ::SQLBindCol(m_hstmt, m_nBindCol, SQL_C_ULONG, &nValue, sizeof(unsigned int), &m_cbValue[m_nBindParm]);
}

SQLRETURN CDBSqlServer::BindCol(bool& nValue)
{
	m_nBindCol++;
	return ::SQLBindCol(m_hstmt, m_nBindCol, SQL_C_BIT, &nValue, sizeof(SQL_C_BIT), &m_cbValue[m_nBindParm]);
}

SQLRETURN CDBSqlServer::BindCol(char* szBuf, int szBufSize)
{
	m_nBindCol++;
	return ::SQLBindCol(m_hstmt, m_nBindCol, SQL_C_CHAR, szBuf, szBufSize, &m_cbValue[m_nBindParm]);
}

SQLRETURN CDBSqlServer::BindCol(TIMESTAMP_STRUCT& timeStamp)
{
	m_nBindCol++;
	return ::SQLBindCol(m_hstmt, m_nBindCol, SQL_C_TIMESTAMP, &timeStamp, sizeof(TIMESTAMP_STRUCT), &m_cbValue[m_nBindParm]);
}



void CDBSqlServer::ClearMoreResult(bool bCloseCursor)
{
	while ((m_retCode = ::SQLMoreResults(m_hstmt)) != SQL_NO_DATA);

	if (bCloseCursor)
	{
		m_retCode = ::SQLCloseCursor(m_hstmt);
	}
}



