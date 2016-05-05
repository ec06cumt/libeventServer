
#pragma once

#include <Windows.h>

#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>
//

#define MAX_CONN_LEN 8192   //连接字符串最大长度
#define MAX_PARM_COUNT 200	//最大的参数个数

#define ODBC_SUCCEED(X) ((SQL_SUCCESS == (X))||(SQL_SUCCESS_WITH_INFO == (X)))
#define ODBC_FAILED(X)  ((SQL_ERROR) == (X) || SQL_INVALID_HANDLE == (X))

class CDBSqlServer
{
public:
	CDBSqlServer();
	~CDBSqlServer();

	bool IsConnected() { return m_bConnected; };
	HENV GetHENV(){ return m_henv; };
	HDBC GetHDBC(){ return m_hdbc; };
	HSTMT GetHSTMT(){ return m_hstmt; };
	SQLWCHAR* CharToWchar(const char* c);

	bool StarUpSqlServer();
	bool ConnectSqlServer(SQLWCHAR* pszDSN, SQLWCHAR* pszPID, SQLWCHAR* pszPW);
	bool DriverConnectSqlServer(SQLWCHAR* pszDSN);
	void DisconnectSqlServer();
	void CleanUp();

	void ClearMoreResult(bool bCloseCursor = false);
	bool ExecuteDirect(SQLWCHAR* sql);

	bool Fetch();

	void GetError();

	void InitBindParam();
	SQLRETURN BindParam(INT8& nValue,SQLSMALLINT inoutType = SQL_PARAM_INPUT);
	SQLRETURN BindParam(UINT8& nValue, SQLSMALLINT inoutType = SQL_PARAM_INPUT);
	SQLRETURN BindParam(int& nValue, SQLSMALLINT inoutType = SQL_PARAM_INPUT);
	SQLRETURN BindParam(char* pstr, int iParamLen, SQLSMALLINT inoutType  = SQL_PARAM_INPUT);

	void InitBindCol();
	SQLRETURN BindCol(INT8& nValue);
	SQLRETURN BindCol(int& nValue);
	SQLRETURN BindCol(unsigned int& nValue);
	SQLRETURN BindCol(bool& nValue);
	SQLRETURN BindCol(char* szBuf,int szBufSize);
	//SQLRETURN BindCol(TCHAR* szBuf, int szBufSize);
	SQLRETURN BindCol(TIMESTAMP_STRUCT& timeStamp );


	
protected:
	HENV m_henv;
	HDBC m_hdbc;
	HSTMT m_hstmt;
	SQLUSMALLINT m_nBindParm;
	SQLUSMALLINT m_nBindCol;

	SQLREAL m_retCode;
	SQLINTEGER m_cbValue[200];
	//SQLLEN m_cbValue[200];			//X64位机器

	TCHAR m_szConnectionStr[MAX_CONN_LEN];
	bool m_bConnected;

	int m_nCursorErrorCount;
};